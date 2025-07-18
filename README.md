# ATtiny13
various Attiny13 MCU projects

Snowflake projects:-
Firmware for ATtiny13a based LED DIY Christmas ornaments "Gikfun Snowflake Shape SMD SMT Welding Practice Soldering Skill Training Board DIY Kit for Arduino Christmas Tree Decoration Gift (Case Pack of 6) GK1018U "

Gikfun listing http://www.gikfun.com/electronic-diy-kits-c-7/snowflake-shape-smd-smt-soldering-skill-training-board-diy-kit-p-825.html
Also available from Amazon:- https://www.amazon.co.uk/gp/product/B07JFWJZ3G

This makes use of The MicroCore ATtiny board definitions which is required to build and upload firmware using Arduino IDE.
You can get MicroCore Here https://github.com/MCUdude/MicroCore. Suitable for any ATtiny13 based 5/6 pin LED project.

Snow3:- 8 LED pattern lightshow utilising simple binary switching, includes various chase, flash and strobe effects.

pwm_snowflake_v1_1:-  5 pattern lightshow utilising software PWM for smoother, more relaxed patterns. 

Light patterns can be selected by cycling the unit's power to advance to the next pattern.
Precompiled binaries (HEX files) included for those that don't want to build their own code and just want to upload Firmware.

## Build test

A helper script in `tests/build.sh` compiles the sketches using `arduino-cli`.
Install `arduino-cli` and the MicroCore board definitions, then run:

```
./tests/build.sh
```

The command fails if either sketch fails to compile.
