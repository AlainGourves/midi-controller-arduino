#include <Arduino.h>
#include "MyLed.h"

MyLed::MyLed(int pin) {
  _pin = pin;
  init();
}

void MyLed::on() {
  digitalWrite(_pin, HIGH);
}

void MyLed::off() {
  digitalWrite(_pin, LOW);
}

void MyLed::init() {
  pinMode(_pin, OUTPUT);
  off();
}

int MyLed::getState() {
  return _ledState;
}

void MyLed::setState(int state_) {
  _ledState = state_;
}

void MyLed::update() {
  unsigned long currentMillis = millis();
  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;

    _ledState = !_ledState;

    if (_ledState == HIGH) {
      on();
    } else {
      off();
    }
  }
}

void MyLed::blink(long rate) {
  _interval = rate;
  update();
}

void MyLed::blink(MyLed led, long rate, int iterations){
  static int count = iterations;
  if (count > 0) {
    blink(rate);
    int st = (led.getState() == 0) ? 1:0;
    led.setState(st);
    count--;
  }
}
