#ifndef MyControl_h
#define MyControl_h

#include <Arduino.h>
// #include "MyGlobals.h"

/* Encoder Library
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 */
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include "MyButton.h"
#include "MyLed.h"


class MyControl {

private:
  uint8_t encoderPin1, encoderPin2, switchPin1, switchPin2, ledPin1, ledPin2, channel;
  bool isActive;
  bool turbo;  // Values incremented by +/-1 when false, +/-10 when true
  MyButton sw1, sw2;
  MyLed greenLed, redLed;
  Encoder myEnc;
  uint8_t increment;
  long oldPosition;
  bool isBlinking;

  // Timer
  unsigned long myTimer;
  static const long ACTIVE_SENSING_PERIOD = 10000;

  void setLeds();


public:
  MyControl(uint8_t encoderPin1, uint8_t encoderPin2, uint8_t switchPin1, uint8_t switchPin2, uint8_t ledPin1, uint8_t ledPin2, uint8_t channel);
  void init();
  void setChannel(uint8_t);
  uint8_t getChannel();
  void setActivity(bool);
  bool getActivity();
  void setIncrement(uint8_t);
  uint8_t getIncrement();
  void update();
  void activate();
  void desactivate();
  void reinitTimer();
  void sendMessage();
  void blink();
};

#endif