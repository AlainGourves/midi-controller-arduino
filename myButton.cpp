#include "MyButton.h"

MyButton::MyButton(int pin) {
  _pin = pin;
  _lastReading = HIGH;  // high by default with pull up
  init();
}

void MyButton::init() {
  pinMode(_pin, INPUT_PULLUP);
  update();
}

void MyButton::update() {
  int newReading = digitalRead(_pin);

  if (newReading != _lastReading) {
    _ignoreUntil = millis() + _debounceDelay;
  }

  if (millis() > _ignoreUntil) {
    if (newReading != _state) {
      _state = newReading;
      _has_changed = true;
    }
  }
  _lastReading = newReading;
}

int MyButton::getState() {
  update();
  return _state;
}

bool MyButton::hasChanged() {
  if (_has_changed) {
		_has_changed = false;
		return true;
	}
	return false;
}

bool MyButton::isPressed() {
  return (getState() == LOW && hasChanged());
}