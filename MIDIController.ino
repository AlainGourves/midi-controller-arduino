#include "MyControl.h"
#include "MyGlobals.h"

// Timer
unsigned long previousTimer;
const long ACTIVE_SENSING_PERIOD = 10000;

MyControl ctrlr1(3, 2, 4, 5, 6, 7, 10);
// MyControl ctrlr2(3, 2, 4, 5, 6, 7, 11);
// MyControl ctrlr3(3, 2, 4, 5, 6, 7 ,12);

MyControl *controllers[1] = {&ctrlr1};
// MyControl controllers[3] = {&ctrlr1, &ctrlr2, &ctrlr3};

/* ------------------------------------------------------DEBUGGING-----------*/
#define DEBUG true  // flag to turn on/off debugging, true to have Serial
#ifdef DEBUG
#define Serial Serial
#endif
/* ------------------------------------------------------DEBUGGING-----------*/

static void OnControlChange(byte channel, byte number, byte value) {
  // Callback for received MIDI messages
  Serial.print(F("ControlChange from channel: "));
  Serial.print(channel);
  Serial.print(F(", number: "));
  Serial.print(number);
  Serial.print(F(", value: "));
  Serial.println(value);

  int idx = findByChannelNumber(channel);

  switch (number) {
    // Manage:
    // - channel attribution for each controller (0x10 + channel number in hex)
    // - controller activation (0x20)
    // - controller desactivation (0x30)
    // - "active sensing" (0x40) : make sure the connection is alive
    case 20:
      if (value == 0x10) {  // DEC 16
        int newChannel = findUnusedController();
        if (newChannel != -1) {
          MIDI.sendControlChange(
              20, newChannel - 1,
              newChannel);  // ControlNumber, ControlValue, Channel
        } else {
          // envoyer un message pour dire qu'il n'y a plus de controller libre
          // TODO in JS
          MIDI.sendControlChange(
              20, 0, channel);  // ControlNumber, ControlValue, Channel
        }
      } else if (value == 0x20 && idx != -1) {  // DEC 32
        // Activation of the controller and confirmation message
        controllers[idx]->activate();
        MIDI.sendControlChange(20, 1, channel);
      } else if (value == 0x30 && idx != -1) {  // DEC 48
        // Desactivation (no response)
        controllers[idx]->desactivate();
      } else if (value == 0x40 && idx != -1) {  // DEC 64
        // pseudo Active Sensing message (no response)
        controllers[idx]->reinitTimer();
      }
      break;
    default:
      Serial.println(F("Message received"));
      break;
  }
}

void setup() {
  Serial.begin(31250);  // MIDI baud rate (useful ?)
#if DEBUG == true
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }
#endif

  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  MIDI.turnThruOff();

  MIDI.setHandleControlChange(OnControlChange);

  // Controller initiation
  for (uint8_t i = 0; i < sizeof(controllers) / sizeof(controllers[0]); i++) {
    controllers[i]->init();
  }

  Serial.println(F("Arduino ready!"));
}

void loop() {
  // MIDI ----------
  MIDI.read();

  for (uint8_t i = 0; i < sizeof(controllers) / sizeof(controllers[0]); i++) {
    controllers[i]->update();
    int incr = controllers[i]->getIncrement();
    if (incr != 0) {
      if (incr == -1) {
        // reset to the original value
        MIDI.sendControlChange(21, 0x28, controllers[i]->getChannel());
      }
      MIDI.sendControlChange(21, incr, controllers[i]->getChannel());
      controllers[i]->reinitTimer();
    }
  }
}

int findUnusedController() {
  for (int i = 0; i < 3; i++) {
    if (controllers[i]->getActivity() == false) {
      return controllers[i]->getChannel();
    }
  }
  return -1;  // No more free controller
}

int findByChannelNumber(int channel_) {
  for (int i = 0; i < 3; i++) {
    if (controllers[i]->getChannel() == channel_) {
      return i;
    }
  }
  return -1;  // No controller
}