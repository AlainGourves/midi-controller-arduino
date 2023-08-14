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

struct Bob {
  int id;
  bool isUsed;
} theChannels[3] = {
  { 10, false }, { 11, false }, { 12, false }
};
int channelsLength = 3;  // stores channels length

int findUnusedChannel() {
  for (int i = 0; i < channelsLength; i++) {
    if (theChannels[i].isUsed == false) {
      return i;
    }
  }
}

int findByChannelNumber(int ch) {
  for (int i = 0; i < channelsLength; i++) {
    if (theChannels[i].id == ch) {
      return i;
    }
  }
}

int channels[3] = { 10, 11, 12 };  // reserved channels for each controller
int idxChannel = 0;                // index of the next channel to use
int myChannel = 1;

bool isControllerActive = false;
bool turbo = false;  // Values incremented by +/-1 when false, +/-10 when true

MyButton sw1(4);  // encoder's switch
MyButton sw2(5);  //momentary switch
MyLed greenLed(6);
MyLed redLed(7);

// Timer
unsigned long previousTimer;
const long ACTIVE_SENSING_PERIOD = 10000;

/* -----------------------------------------------------------------------------------------DEBUGGING-----------*/
#define DEBUG true  // flag to turn on/off debugging, true to have Serial
#ifdef DEBUG
#define Serial Serial
#endif
/* -----------------------------------------------------------------------------------------DEBUGGING-----------*/

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 2);  // INT0 & INT 1

long oldPosition = 0;  // Encoder's position

void blink() {
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

static void OnControlChange(byte channel, byte number, byte value) {
  Serial.print(F("ControlChange from channel: "));
  Serial.print(channel);
  Serial.print(F(", number: "));
  Serial.print(number);
  Serial.print(F(", value: "));
  Serial.println(value);
  switch (number) {
    case 20:
      // Manage:
      // - channel attribution for each controller (0x10 + channel number in hex)
      // - controller activation (0x20)
      // - controller desactivation (0x30)
      if (value == 0x10) {
        int id = findUnusedChannel();
        myChannel = theChannels[id].id;
        theChannels[id].isUsed = true;
        MIDI.sendControlChange(20, myChannel - 1, myChannel);
      } else if (value == 0x20) {
        // active le bazar
        isControllerActive = true;
        MIDI.sendControlChange(20, 1, myChannel);
        // starts timer
        previousTimer = millis();
        blink();
      } else if (value == 0x30) {
        // désactive le bazar
        // update channels array
        int idx = findByChannelNumber(channel);
        theChannels[idx].isUsed = false;
        isControllerActive = false;
        if (turbo) {
          redLed.off();
          turbo = false;
        }
      } else if (value == 0x40) {
        // pseudo Active Sensing message
        // Reinit timer
        previousTimer = millis();
      }
      break;
    default:
      Serial.println(F("message reçu"));
      break;
  }
}

void setup() {
  Serial.begin(31250);  // MIDI baud rate
#if DEBUG == true
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }
#endif

  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
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
      MIDI.sendControlChange(21, val, myChannel);
      // Reinit timer
      previousTimer = millis();
    }
  }

  // Momentary Switches -----------
  if (sw1.isPressed()) {
    // Encoder'switch-> send message to reset to initial value
    MIDI.sendControlChange(21, 40, myChannel);
  }

  if (sw2.isPressed() && isControllerActive) {
    turbo = !turbo;
  }

  // Timer
  unsigned long elapsed = millis() - previousTimer;
  if (elapsed >= ACTIVE_SENSING_PERIOD && isControllerActive) {
    Serial.println(F("Le contrôleur doit être dans les choux!"));
    isControllerActive = false;
    // update channels array
    int idx = findByChannelNumber(myChannel);
    theChannels[idx].isUsed = false;
  }

  // leds status
  if (isControllerActive) {
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
