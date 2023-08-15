#include <Arduino.h>
#include "MyControl.h"
#include "MyButton.h"
#include "MyLed.h"
// #include <Encoder.h>


MyControl::MyControl(int encoderPin1, int encoderPin2, int switchPin1, int switchPin2, int ledPin1, int ledPin2)
  : sw1(switchPin1), sw2(switchPin2), greenLed(ledPin1), redLed(ledPin2)
// MyControl::MyControl(int encoderPin1, int encoderPin2, int switchPin1, int switchPin2, int ledPin1, int ledPin2) :  myEnc(encoderPin1, encoderPin2), sw1(switchPin1), sw2(switchPin2), greenLed(ledPin1), redLed(ledPin2)
{
  init();
}

void MyControl::init() {
  // default values
  this->channel = 1;
  this->isActive = false;
  this->turbo = false;
}

void MyControl::setChannel(int channel) {
  this->channel = channel;
}

void MyControl::setActivity(bool val) {
  this->isActive = val;
}

int MyControl::getChannel() {
  return channel;
}

bool MyControl::getActivity() {
  return isActive;
}

void MyControl::update() {
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

  setLeds();
}

void MyControl::activate() {
  isActive = true;
  myTimer = millis();
  blink();
}

void MyControl::desactivate() {
  isActive = false;
  myTimer = 0;
  if (turbo) {
    redLed.off();
    turbo = false;
  }
}

void MyControl::setLeds(){
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
  greenLed.on();
  delay(50);
  greenLed.off();
  redLed.on();
  delay(50);
  greenLed.on();
  redLed.off();
  delay(50);
  greenLed.off();
  redLed.on();
  delay(50);
  greenLed.on();
  redLed.off();
  delay(50);
  greenLed.off();
  redLed.on();
  delay(50);
  redLed.off();
}