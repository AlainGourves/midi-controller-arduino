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
  uint8_t _encoderPin1, _encoderPin2, _switchPin1, _switchPin2, _ledPin1, _ledPin2, _channel;
  bool _isActive;
  bool _turbo;  // Values incremented by +/-1 when false, +/-10 when true
  MyButton _sw1, _sw2;
  MyLed _greenLed, _redLed;
  Encoder _myEnc;
  int _increment;
  long _oldPosition;
  bool _isBlinking;

  void setIncrement(int);

  // Timer
  unsigned long _myTimer;
  static const long ACTIVE_SENSING_PERIOD = 10000;
  void setTimer();

  void setLeds();
  void blink();

 public:
  MyControl(uint8_t _encoderPin1, uint8_t _encoderPin2, uint8_t _switchPin1,
            uint8_t _switchPin2, uint8_t _ledPin1, uint8_t _ledPin2,
            uint8_t _channel);
  void init();
  unsigned long getTimer();
  void setChannel(uint8_t);
  uint8_t getChannel();
  void setActivity(bool);
  bool getActivity();
  int getIncrement();
  void update();
  void activate();
  void desactivate();
  void reinitTimer();
};

#endif