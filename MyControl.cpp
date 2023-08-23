#include <Arduino.h>

// #include "MyGlobals.h"
#include "MyControl.h"
#include <Encoder.h>
#include "MyButton.h"
#include "MyLed.h"

// MyControl::MyControl(int encoderPin1, int encoderPin2, int switchPin1, int
// switchPin2, int ledPin1, int ledPin2)
//   : sw1(switchPin1), sw2(switchPin2), greenLed(ledPin1), redLed(ledPin2)
MyControl::MyControl(uint8_t encoderPin1, uint8_t encoderPin2, uint8_t switchPin1,
                     uint8_t switchPin2, uint8_t ledPin1, uint8_t ledPin2, uint8_t channel)
    : myEnc(encoderPin1, encoderPin2),
      sw1(switchPin1),
      sw2(switchPin2),
      greenLed(ledPin1),
      redLed(ledPin2),
      channel(channel) {
  init();
}

void MyControl::init() {
  // default values
  isActive = false;
  turbo = false;
  isBlinking = false;
  long oldPosition = 0;
}

void MyControl::setChannel(uint8_t channel_) { channel = channel_; }

void MyControl::setActivity(bool val_) { isActive = val_; }

void MyControl::setIncrement(uint8_t incr_) { increment = incr_; }

uint8_t MyControl::getChannel() { return channel; }

bool MyControl::getActivity() { return isActive; }

uint8_t MyControl::getIncrement() { return increment; }

void MyControl::update() {
  // Encoder ----------
  if (isActive) {
    long newPosition = myEnc.read() >> 2;  // encoder reads 1 step as 4
    if (newPosition != oldPosition) {
      int diff = newPosition - oldPosition;
      if (diff > 0) setIncrement(1);
      if (diff < 0) setIncrement(3);
      if (diff > 0 && turbo) setIncrement(10);
      if (diff < 0 && turbo) setIncrement(30);
      oldPosition = newPosition;
      // TODO: Send MIDI message
      //  MIDI.sendControlChange(21, getIncrement(), getChannel());
    } else {
      setIncrement(0);
    }
  } else {
    setIncrement(0);
  }

  // Momentary Switches -----------
  if (sw1.isPressed()) {
    if (isActive) {
      desactivate();
    } else {
      activate();
    }
  }
  if (sw2.isPressed() && isActive) {
    turbo = !turbo;
  }

  // Timer
  unsigned long elapsed = millis() - myTimer;
  if (elapsed >= ACTIVE_SENSING_PERIOD && isActive) {
    Serial.println(F("Le contrôleur doit être dans les choux!"));
    desactivate();
  }

  if (isBlinking) {
    if (greenLed.getBlinking()) {
      blink();
    } else {
      isBlinking = false;
    }
  } else {
    setLeds();
  }
}

void MyControl::activate() {
  setActivity(true);
  myTimer = millis();
  blink();
  // Confirmation message
  MIDI.sendControlChange(20, 1, channel);
}

void MyControl::desactivate() {
  setActivity(false);
  myTimer = 0;
  if (turbo) {
    redLed.off();
    turbo = false;
  }
}

void MyControl::sendMessage() {
  // envoyer la position de l'encoder

  reinitTimer();  //  each sent message reinitializes the timer
}

void MyControl::reinitTimer() { myTimer = millis(); }

void MyControl::setLeds() {
  // leds status
  if (isActive) {
    greenLed.on();
  } else {
    greenLed.off();
  }
  if (turbo) {
    redLed.on();
  } else {
    redLed.off();
  }
}

void MyControl::blink() {
  isBlinking = true;
  greenLed.blink(redLed, 50, 11);
}