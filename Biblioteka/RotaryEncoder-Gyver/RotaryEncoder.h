// -----
// RotaryEncoder.h - Library for using rotary encoders.
// This class is implemented for use with the Arduino environment.
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// -----

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include "Arduino.h"

#define LATCHSTATE 3

class RotaryEncoder
{
public:
  // ----- Constructor -----
  RotaryEncoder(uint8_t pin1, uint8_t pin2, bool dir, bool type);
  
  // retrieve the current position
  long  getPosition();

  // adjust the current position
  void setPosition(long newPosition);

  // call this function every some milliseconds or by using an interrupt for handling state changes of the rotary encoder.
  void tick(void);

private:
  uint8_t _pin1, _pin2; // Arduino pins used for the encoder. 
  
  int8_t _oldState;
  bool _type = false, _turnFlag = true;
  long _position;     // Internal position (4 times _positionExt)
  long _positionExt;  // External position
};

#endif

// End