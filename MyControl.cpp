#include <Arduino.h>

// #include "MyGlobals.h"
#include <Encoder.h>

#include "MyButton.h"
#include "MyControl.h"
#include "MyLed.h"

// MyControl::MyControl(int encoderPin1, int encoderPin2, int switchPin1, int
// switchPin2, int ledPin1, int ledPin2)
//   : sw1(switchPin1), sw2(switchPin2), greenLed(ledPin1), redLed(ledPin2)
MyControl::MyControl(uint8_t _encoderPin1, uint8_t _encoderPin2,
                     uint8_t _switchPin1, uint8_t _switchPin2, uint8_t _ledPin1,
                     uint8_t _ledPin2, uint8_t _channel)
    : _myEnc(_encoderPin1, _encoderPin2),
      _sw1(_switchPin1),
      _sw2(_switchPin2),
      _greenLed(_ledPin1),
      _redLed(_ledPin2),
      _channel(_channel) {
  init();
}

void MyControl::init() {
  // default values
  _isActive = false;
  _turbo = false;
  _isBlinking = false;
  _increment = 0;
  _myTimer = 0;
  _oldPosition = 0;
}

void MyControl::setChannel(uint8_t channel_) { _channel = channel_; }

void MyControl::setActivity(bool val_) { _isActive = val_; }

void MyControl::setIncrement(int incr_) { _increment = incr_; }

uint8_t MyControl::getChannel() { return _channel; }

bool MyControl::getActivity() { return _isActive; }

int MyControl::getIncrement() { return _increment; }

void MyControl::setTimer() { _myTimer = millis(); }

unsigned long MyControl::getTimer() { return _myTimer; }

void MyControl::update() {
  // Encoder ----------
  if (_isActive) {
    long newPosition = _myEnc.read() >> 2;  // encoder reads 1 step as 4
    if (newPosition != _oldPosition) {
      int diff = newPosition - _oldPosition;
      if (diff > 0) setIncrement(1);
      if (diff < 0) setIncrement(3);
      if (diff > 0 && _turbo) setIncrement(10);
      if (diff < 0 && _turbo) setIncrement(30);
      _oldPosition = newPosition;
      // TODO: Send MIDI message
      //  MIDI.sendControlChange(21, getIncrement(), getChannel());
    } else {
      setIncrement(0);
    }
  } else {
    setIncrement(0);
  }

  // Momentary Switches -----------
  if (_sw1.isPressed()) {
    // -1: Signal to send message to reinit increment value to default
    setIncrement(-1);
  }
  if (_sw2.isPressed() && _isActive) {
    _turbo = !_turbo;
  }

  // Timer
  unsigned long elapsed = millis() - getTimer();
  if (elapsed >= ACTIVE_SENSING_PERIOD && _isActive) {
    Serial.println(F("Le contrôleur doit être dans les choux!"));
    desactivate();
  }

  if (_isBlinking) {
    if (_greenLed.getBlinking()) {
      blink();
    } else {
      _isBlinking = false;
    }
  } else {
    setLeds();
  }
}

void MyControl::activate() {
  setActivity(true);
  setTimer();
  blink();
  // Confirmation message
  // MIDI.sendControlChange(20, 1, channel);
}

void MyControl::desactivate() {
  setActivity(false);
  if (_turbo) {
    _redLed.off();
    _turbo = false;
  }
}

void MyControl::reinitTimer() { setTimer(); }

void MyControl::setLeds() {
  // leds status
  if (_isActive) {
    _greenLed.on();
  } else {
    _greenLed.off();
  }
  if (_turbo) {
    _redLed.on();
  } else {
    _redLed.off();
  }
}

void MyControl::blink() {
  _isBlinking = true;
  _greenLed.blink(_redLed, 60, 9); // 9 alternate blinks of 60ms
}