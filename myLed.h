#ifndef MyLed_h
#define MyLed_h

#include <Arduino.h>


class MyLed {
public:
  MyLed(int pin);
  void init();
  void update();
  void blink(long rate);
  void blink(MyLed led, long rate, int iterations);
  void on();
  void off();
  int getState();
  void setState(int state_);
private:
  int _pin;
  int _ledState;
  unsigned long _previousMillis;
  unsigned long _interval;
};

#endif