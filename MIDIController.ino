// MIDI
#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();
#include "MyControl.h"

struct Bob {
  int id;
  bool isUsed;
} theChannels[3] = {
  { 10, false }, { 11, false }, { 12, false } // reserved channels for each controller
};
int channelsLength = 3;  // stores channels length

int findUnusedChannel() {
  // TODO: error handling (no more free channel)
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

int myChannel = 1;

// Timer
unsigned long previousTimer;
const long ACTIVE_SENSING_PERIOD = 10000;

/* -----------------------------------------------------------------------------------------DEBUGGING-----------*/
#define DEBUG true  // flag to turn on/off debugging, true to have Serial
#ifdef DEBUG
#define Serial Serial
#endif
/* -----------------------------------------------------------------------------------------DEBUGGING-----------*/

/*
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
*/

MyControl controller1(3,2,4,5,6,7);

void setup() {
  Serial.begin(31250);  // MIDI baud rate
#if DEBUG == true
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }
#endif

  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  MIDI.turnThruOff();
  // MIDI.setHandleControlChange(OnControlChange);

  controller1.activate();

  Serial.println(F("Arduino ready!"));
}

void loop() {
  // MIDI ----------
  MIDI.read();

controller1.update();
/*
  // Timer
  unsigned long elapsed = millis() - previousTimer;
  if (elapsed >= ACTIVE_SENSING_PERIOD && isControllerActive) {
    Serial.println(F("Le contrôleur doit être dans les choux!"));
    isControllerActive = false;
    // update channels array
    int idx = findByChannelNumber(myChannel);
    theChannels[idx].isUsed = false;
  }
*/
}
