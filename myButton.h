#ifndef MyButton_h
#define MyButton_h

#include <Arduino.h>

class MyButton {

private:
  int _pin;
  int _state;
  int _lastReading;
  bool _has_changed;
  unsigned long _ignoreUntil;
  const unsigned long _debounceDelay = 50;

public:
  MyButton(int pin);

  void init();
  void update();
  int getState();
  bool isPressed();
  bool hasChanged();
};

#endif