//CC BY-SA: This license allows reusers to distribute, remix, adapt, and build upon the material in any medium or format,
//so long as attribution is given to the creator. The license allows for commercial use.
//If you remix, adapt, or build upon the material, you must license the modified material under identical terms.

//Except as represented in this agreement, all work product by Developer is provided ​“AS IS”. Other than as provided in this agreement,
//Developer makes no other warranties, express or implied, and hereby disclaims all implied warranties,
//including any warranty of merchantability and warranty of fitness for a particular purpose.

// Created by Richard Beech 2021

char addressConfigValue; //Config value
char addressConfigValueLimit = 7;

int pinOut[] = {
  0, 1, 2, 3, 4
};
byte countOut = 5;
byte timerOut = 200;
byte countLed = 0;
boolean ledON = HIGH;
byte phase = 0;
byte cutOff = 0;
unsigned long ticker = 0;

void iniConfig() {
  addressConfigValue = eeprom_read(0);
  addressConfigValue++;
  if (addressConfigValue > addressConfigValueLimit) addressConfigValue = 0;
  eeprom_write(0, addressConfigValue);
}

void setup()
{
  DDRB = 0B111111; //set all pins as output
  iniConfig();
  showConfig(); //Blink mode times
}



void fadeOut (int led, int rate) {
  digitalWrite(pinOut[led], 0);
  // wait for 30 milliseconds to see the dimming effect
}

//inverse chase, all LEDs lit except one
void inverseChase (int countLed, int rate) {
  for (int i = 0; i < countOut; i++) {
    digitalWrite(pinOut[i], ledON);
  }
  digitalWrite(pinOut[countLed], !ledON);
  delay(timerOut / rate);
}

//chase. all LEDs off apart from 1
void chase (int countLed, int rate) {
  for (int i = 0; i < countOut; i++) {
    digitalWrite(pinOut[i], !ledON);
  }
  digitalWrite(pinOut[countLed], ledON);
  delay(timerOut / rate);
}

//all on/off
void allOnOff(int state, int rate) {
  for (int i = 0; i < countOut; i++) {
    if (state == 1)
      digitalWrite(pinOut[i], ledON);
    else
      digitalWrite(pinOut[i], !ledON);
  }
  delay(rate);
}


void loop()
{
  switch (addressConfigValue) {
    case 0:
      //slow inverse chase
      inverseChase(countLed, 1);
      break;
    //fast inverse chase
    case 1:
      inverseChase(countLed, 4);
      break;
    //slow chase
    case 2:
      chase(countLed, 1);
      break;
    //fast chase
    case 3:
      chase(countLed, 4);
      break;
    //blink all (1 second cycle)
    case 4:
      allOnOff(1, 1000);
      allOnOff(0, 1000);
      break;
    //flash all (.1 secondcycle)
    case 5:
      allOnOff(1, 100);
      allOnOff(0, 100);
      break;
    //strobe all (.05 second cycle)
    case 6:
      allOnOff(1, 50);
      allOnOff(0, 50);
      break;
    //fast strobe all (.02 second cycle)
    case 7:
      allOnOff(1, 20);
      allOnOff(0, 20);
      break;
  }

  //main LED iterator
  countLed ++;
  if ( countLed >= 5) countLed = 0;
}

//flashes one LED to show which number the current pattern is
void showConfig() {
  //digitalWrite(3, LOW);
  PORTB &= ~_BV(3); //LOW
  delay(500);
  for (char i = 0; i < addressConfigValue ; i++) {
    PORTB |= _BV(3); //HIGH
    delay(200);
    PORTB &= ~_BV(3); //LOW
    delay(200);
  }
}

//EEPROM write data
void eeprom_write(unsigned char addr, unsigned char wdata)
{
  cli();
  while (EECR & (1 << EEWE));
  EECR = (0 << EEPM1) | (0 >> EEPM0);
  EEAR = addr;
  EEDR = wdata;
  EECR |= (1 << EEMWE);
  EECR |= (1 << EEWE);
  sei();
}

//EEPROM read data
unsigned char eeprom_read(unsigned char addr)
{
  while (EECR & (1 << EEWE));
  EEAR = addr;
  EECR |= (1 << EERE);
  return EEDR;
}
