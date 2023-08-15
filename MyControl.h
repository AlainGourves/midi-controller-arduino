#pragma once

#ifndef MyControl_h
#define MyControl_h

#include <Arduino.h>

/* Encoder Library 
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 */
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
// #define ENCODER_OPTIMIZE_INTERRUPTS
// #include <Encoder.h>

#include "MyButton.h"
#include "MyLed.h"

class MyControl {

private:
  int encoderPin1, encoderPin2, switchPin1, switchPin2, ledPin1, ledPin2;
  int channel;
  bool isActive;
  bool turbo;  // Values incremented by +/-1 when false, +/-10 when true
  MyButton sw1, sw2;
  MyLed greenLed, redLed;
  // Encoder myEnc;

  // Timer
  unsigned long myTimer;
  static const long ACTIVE_SENSING_PERIOD = 10000;

  void setLeds();


public:
  MyControl(int encoderPin1, int encoderPin2, int switchPin1, int switchPin2, int ledPin1, int ledPin2);
  void init();
  void setChannel(int);
  void setActivity(bool);
  int getChannel();
  bool getActivity();
  void update();
  void activate();
  void desactivate();
  void blink();
};

#endif