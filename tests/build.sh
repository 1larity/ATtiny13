#!/bin/sh
set -e

FQBN="attiny:avr:ATtiny13:clock=9MHz_internal"

arduino-cli compile --fqbn "$FQBN" Snow3
arduino-cli compile --fqbn "$FQBN" pwm_snowflake_v1_1
