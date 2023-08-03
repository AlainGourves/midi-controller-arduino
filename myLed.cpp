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