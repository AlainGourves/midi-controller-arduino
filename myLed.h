#ifndef MyLed_h
#define MyLed_h
#include <Arduino.h>

class MyLed {
 public:
  MyLed(int pin);
  void init();
  void update();
  void loop();
  void blink(long rate);  // Single LED blinking
  void blink(MyLed& led, long rate,
             int iterations);  // for 2 LEDs alternate blinking
  bool getBlinking();
  void on();
  void off();
  int getState();
  void setState(int state_);

 private:
  int _pin;
  int _ledState;
  bool _isBlinking;
  int _blinkIterations;
  unsigned long _previousMillis;
  unsigned long _interval;
};

#endif