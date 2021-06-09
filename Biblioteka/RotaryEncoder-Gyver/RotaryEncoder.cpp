// -----
// RotaryEncoder.cpp - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// 17.06.2015 minor updates.
// -----

#include "Arduino.h"
#include "RotaryEncoder.h"


// The array holds the values –1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
0,  1, -1,  0  };


// positions: [3] 1 0 2 [3] 1 0 2 [3]
// [3] is the positions where my rotary switch detends
// ==> right, count up
// <== left,  count down


// ----- Initialization and Default Values -----

RotaryEncoder::RotaryEncoder(uint8_t pin1, uint8_t pin2, bool dir, bool type) {
  
  // Remember Hardware Setup
  if (dir) {
	 _pin1 = pin1;
     _pin2 = pin2;
	} else {
	 _pin1 = pin2;
     _pin2 = pin1;
	}		 
  _type = type;
  // Setup the input pins
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  digitalWrite(pin2, HIGH);   // turn on pullup resistor
  
  // when not started in motion, the current state of the encoder should be 3
  _oldState = 3;

  // start with position 0;
  _position = 0;
  _positionExt = 0;
} // RotaryEncoder()


long  RotaryEncoder::getPosition() {
  return _positionExt;
} // getPosition()


void RotaryEncoder::setPosition(long newPosition) {
  // only adjust the external part of the position.
  _position = ((newPosition<<2) | (_position & 0x03L));
  _positionExt = newPosition;
} // setPosition()


void RotaryEncoder::tick(void)
{
//  int sig2 = digitalRead(_pin2);
//  int8_t thisState = digitalRead(_pin1) | (digitalRead(_pin2) << 1);
   int8_t thisState = digitalRead(_pin1);

  if (_oldState != thisState) {
	_turnFlag = !_turnFlag;
	if (_turnFlag || !_type) { 
	  if (digitalRead(_pin2) != _oldState)   _positionExt += 1;
	    else _positionExt -= 1;
    }
    _oldState = thisState;
  } // if
} // tick()

// End