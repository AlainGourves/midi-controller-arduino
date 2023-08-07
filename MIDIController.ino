#include "myButton.h"
#include "myLed.h"

/* Encoder Library 
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 */
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

// MIDI
#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();

bool isControllerActive = false;
bool turbo = false;  // Values incremented by +/-1 when false, +/-10 when true

MyButton sw1(4);  // encoder's switch
MyButton sw2(5);  //momentary switch
MyLed greenLed(6);
MyLed redLed(7);

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 2);  // INT0 & INT 1

long oldPosition = 0;  // Encoder's position

void blink() {
  greenLed.on();
  delay(100);
  greenLed.off();
  redLed.on();
  delay(100);
  greenLed.on();
  redLed.off();
  delay(100);
  greenLed.off();
  redLed.on();
  delay(100);
  redLed.off();
}

static void OnControlChange(byte channel, byte number, byte value) {
  // Serial.print(F("ControlChange from channel: "));
  // Serial.print(channel);
  // Serial.print(F(", number: "));
  // Serial.print(number);
  // Serial.print(F(", value: "));
  // Serial.println(value);
  switch (number) {
    case 20:
      if (value == 0x20) {
        // active le bazar
        isControllerActive = true;
        MIDI.sendControlChange(20, 4, 12);
        blink();
      } else if (value == 0x30) {
        // désactive le bazar
        isControllerActive = false;
        if (turbo) {
          redLed.off();
        }
      }
      break;
    default:
      Serial.println(F("message reçu"));
      break;
  }
}

void setup() {
  Serial.begin(31250);  // MIDI baud rate
  while (!Serial)
    ;

  MIDI.begin();
  MIDI.turnThruOff();
  MIDI.setHandleControlChange(OnControlChange);

  Serial.println(F("Arduino ready!"));
}

void loop() {
  // MIDI ----------
  MIDI.read();

  // Encoder ----------
  if (isControllerActive) {
    long newPosition = myEnc.read() >> 2;  // encoder reads 1 step as 4
    if (newPosition != oldPosition) {
      int diff = newPosition - oldPosition;
      int val;
      if (diff > 0) val = 1;
      if (diff < 0) val = 3;
      if (diff > 0 && turbo) val = 10;
      if (diff < 0 && turbo) val = 30;
      oldPosition = newPosition;
      // Send MIDI message
      MIDI.sendControlChange(21, val, 1);
    }
  }

  // Momentary Switches -----------
  if (sw1.isPressed()) {
    // Encoder'switch-> send message to reset to initial value
    MIDI.sendControlChange(22, 1, 1);
  }

  if (sw2.isPressed() && isControllerActive) {
    turbo = !turbo;
    if (turbo) {
      redLed.on();
    } else {
      redLed.off();
    }
  }

  // Green status led
  if (isControllerActive) {
    greenLed.on();
  } else {
    greenLed.off();
  }
}
