//CC BY-SA: This license allows reusers to distribute, remix, adapt, and build upon the material in any medium or format,
//so long as attribution is given to the creator. The license allows for commercial use.
//If you remix, adapt, or build upon the material, you must license the modified material under identical terms.

//Except as represented in this agreement, all work product by Developer is provided ​“AS IS”. Other than as provided in this agreement,
//Developer makes no other warranties, express or implied, and hereby disclaims all implied warranties,
//including any warranty of merchantability and warranty of fitness for a particular purpose.

// Created by Richard Beech 2021



//Soft PWM control variables
volatile byte intCounter = 0; //counts the number of PWM ticks so duty period can be calculated. Volatile so it works in/across interrupts
int  dutyThreshHold = 1; //number of PWM ticks to count before switching duty cycle to ON

//timer controls, so we can do time based calculations without blocking delay() calls.
long currentMillis;  //time based delay, current time
long previousMillis = 0;  //time based delay, last time called
#define increment 1      //update frequency in milliseconds
//LED fade in/out rate
#define cRate 16 //how fast to do fade, smaller = lower
char rate = cRate; //allows fade rate to be decremented (to fade out) or incremented (to fade in).

//mode selection and control vars
byte mode=1; //the currently selected mode
#define maxMode  4 // the highest mode number

//LED selection vars
byte randLED = 0; //value of current randomly selected LED for random modes
byte currentLED = 0; // value of current LED for incrementing/decrementing modes
byte pinOut[] = { 0, 1, 2, 3, 4}; //array of LED ids for looping operations
#define maxLEDs 5// the maximum number of LEDs used +1

//PRNG vars, do not modify
#define BBS_P (13)
#define BBS_Q (97)
#define BBS_SEED (123)
static byte m;
static byte x;
static uint16_t r;



void eeprom_write(unsigned char addr, unsigned char wdata)
{
  cli();
  while (EECR & (1 << EEWE)); 
  EECR = (0 << EEPM1) | (0 >> EEPM0);
  EEAR = addr; 
  EEDR = wdata;
  EECR |= (1 << EEMWE); 
  EECR |= (1 << EEWE); 
}


unsigned char eeprom_read(unsigned char addr)
{
  while (EECR & (1 << EEWE));
  EEAR = addr;
  EECR |= (1 << EERE);
  return EEDR;
}

//get and increment pattern mode from eeprom
void initConfig()
{
  mode = eeprom_read(0);
  mode++;
  if (mode >= maxMode) mode = 0; 
  eeprom_write(0, mode);
}

//timer interrupt routine
ISR(TIM0_COMPA_vect)
{
  intCounter ++ ; //increment duty counter
}

//init PRNG
void bbs_init(uint16_t p, uint16_t q, uint16_t seed)
{
  m = p * q;
  r = x = seed;
}

//generate PRNG
uint16_t bbs_next()
{
  return (r = (r * x) % m);
}

void setup() {
  /* setup */
  initConfig(); //read mode setting from EEPROM
  DDRB = 0b111111111; //set all pins to output
  PORTB = 0b00000000; // set all pins to LOW
  TCCR0B =  1 << CS00;
  TCCR0A |= (1 << WGM01) | (1 << WGM00) ; // set timer mode to FAST PWM
  OCR0A = 10; // compare timer with this value (will cause interrupt)
  TIMSK0 |= (1 << OCIE0A); // enable Timer compare interrupt
  bbs_init(BBS_P, BBS_Q, BBS_SEED); // init PRNG.
  sei(); // enable global interrupts

}

void loop() {
  // put your main code here, to run repeatedly:

  //twinkle random leds
  if (mode == 0) {
    for (int i = 0; i < maxLEDs; i++) {
      if (i != randLED) {
        digitalWrite(pinOut[i], LOW);
      }
    }

    if (intCounter  <= dutyThreshHold) {
      digitalWrite(pinOut[randLED], HIGH);
    } else {
      digitalWrite(pinOut[randLED], LOW);
    }

  }

  //inverted single LED chase
  if (mode == 1) {
    //turn on all LEDs apart from current
    for (int i = 0; i < maxLEDs; i++) {
      if (i != currentLED) {
        digitalWrite(pinOut[i], HIGH);
      }
    }
    //fade current LED out then in
    if (intCounter  >= dutyThreshHold) {
      digitalWrite(pinOut[currentLED], HIGH);
    } else {
      digitalWrite(pinOut[currentLED], LOW);
    }
  }

  //single LED chase
  if (mode == 2) {
    if (intCounter  <= dutyThreshHold) {
      digitalWrite(pinOut[currentLED], HIGH);
    } else {
      digitalWrite(pinOut[currentLED], LOW);
    }
  }

  //fade alternating leds on/off
  if (mode == 3) {
    if (intCounter  <= dutyThreshHold) {
      PORTB = 0b11101101; //  LED pins on
    } else {
      PORTB = 0b00010010; //  LED pins off
    }
  }

  //fade all on/off
  if (mode == 4) {
    if (intCounter  <= dutyThreshHold) {
      PORTB = 0b11111111; //  LED pins on
    } else {
      PORTB = 0b00000000; //  LED pins off
    }
  }


  //stuff we want to do every n milliseconds, such as changing current LED, checking if a fade is complete
  currentMillis = millis();
  if (currentMillis - previousMillis > increment)
  {
    //do fade in/out stuff
    dutyThreshHold += rate; //increment/decremet the on-time threshold (and thus PWM duty cycle)
    if (dutyThreshHold >= 255) {
      rate = -cRate; //if we've reached max duty cycle, set rate to decrement
    }
    if (dutyThreshHold <= 0) {
      rate = cRate; //if we've reached min duty cycle, set rate to increment
      currentLED = currentLED + 1; //advance curent LED

      randLED = (bbs_next()) % maxLEDs;

      if (currentLED >= maxLEDs) { //have we got past the last LED?
        currentLED = 0; //loop around to first LED
      }


    }
    previousMillis = currentMillis; // save the last time you did this (reset timer)
  }

}
