#include "MyGlobals.h"

// MIDI
#include <USB-MIDI.h>
// Create and bind the MIDI interface to the default hardware Serial port
USBMIDI_CREATE_DEFAULT_INSTANCE();

#include "MyControl.h"

// struct Bob {
//   int id;
//   bool isUsed;
// } theChannels[controlsNumber] = {
//   { 10, false }, { 11, false }, { 12, false } // reserved channels for each controller
// };

int findUnusedChannel() {
  // TODO: error handling (no more free channel)
  for (int i = 0; i < controlsNumber; i++) {
    if (theChannels[i].isUsed == false) {
      return i;
    }
  }
}

int findByChannelNumber(int ch) {
  for (int i = 0; i < controlsNumber; i++) {
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


static void OnControlChange(byte channel, byte number, byte value) {
  Serial.print(F("ControlChange from channel: "));
  Serial.print(channel);
  Serial.print(F(", number: "));
  Serial.print(number);
  Serial.print(F(", value: "));
  Serial.println(value);
  // TODO: Find the message's recipient:
  // if (channel != 0) channel = recipient's private channel
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
        // Activation
        // 1) find the message's recipient (his own channel  = param 'channel')
        // TODO
        // 2) ask him to activate itself, and to reply with a confirmation message
      } else if (value == 0x30) {
        // Desactivation
        // update channels array
        // 1) find the message's recipient (his own channel  = param 'channel')
        // TODO
        int idx = findByChannelNumber(channel);
        theChannels[idx].isUsed = false;
      } else if (value == 0x40) {
        // pseudo Active Sensing message
      }
      break;
    default:
      Serial.println(F("message reçu"));
      break;
  }
}


MyControl controller1(3, 2, 4, 5, 6, 7);

void setup() {
  Serial.begin(31250);  // MIDI baud rate
#if DEBUG == true
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }
#endif

  MIDI.begin();  // Listen to all incoming messages
  MIDI.turnThruOff();

  MIDI.setHandleControlChange(OnControlChange);

  controller1.activate();

  Serial.println(F("Arduino ready!"));
}

void loop() {
  // MIDI ----------
  MIDI.read();

  controller1.update();

  int incr = controller1.getIncrement();
  if (incr != 0) {
    Serial.print("Incr: ");
    Serial.println(controller1.getIncrement());
  }
  /*
  // Message
  int a = controller1.midiMsg[0];
  int b = controller1.midiMsg[1];
  int c = controller1.midiMsg[2];
  if (a != 0 || b != 0 || c != 0) {
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.println(c);
    int msg[] = { 0, 0, 0 };
    controller1.setMidiMsg(msg);
  }
  */
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
