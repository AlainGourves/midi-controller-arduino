#include "MyLed.h"

#include <Arduino.h>

MyLed::MyLed(int pin) {
  _pin = pin;
  init();
}

void MyLed::on() { digitalWrite(_pin, HIGH); }

void MyLed::off() { digitalWrite(_pin, LOW); }

void MyLed::init() {
  pinMode(_pin, OUTPUT);
  _previousMillis = 0;
  _interval = 0;
  _blinkIterations = 0;
  _isBlinking = false;
  off();
}

int MyLed::getState() { return _ledState; }

void MyLed::setState(int state_) {
  _ledState = state_;
  if (_ledState == 1) {
    on();
  } else {
    off();
  }
}

bool MyLed::getBlinking() { return _isBlinking; }

void MyLed::update() {
  unsigned long currentMillis = millis();
  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;
    setState(!_ledState);
  }
}

void MyLed::loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - _previousMillis >= _interval && _blinkIterations > 0) {
    _blinkIterations -= 1;
    _previousMillis = currentMillis;
    setState(!_ledState);
  }
  if (_blinkIterations == 0) {
    _isBlinking = false;
  }
}

void MyLed::blink(long rate) {
  _interval = rate;
  update();
}

void MyLed::blink(MyLed const &led, long rate, int iterations) {
  if (!_isBlinking) {
    _blinkIterations = iterations;
  }
  _interval = rate;
  _isBlinking = true;
  int st = getState();
  led.setState(!st);
  loop();
}