#ifndef MyLed_h
#define MyLed_h

#include <Arduino.h>


class MyLed {
public:
  MyLed(int pin);
  void init();
  void update();
  void blink(long rate);
  void on();
  void off();
private:
  int _pin;
  int _ledState;
  unsigned long _previousMillis;
  unsigned long _interval;
};

#endif