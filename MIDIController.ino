#include "MyGlobals.h"
#include "MyControl.h"

int myChannel = 1;

// Timer
unsigned long previousTimer;
const long ACTIVE_SENSING_PERIOD = 10000;


MyControl ctrlr1(3, 2, 4, 5, 6, 7, 10);
// MyControl ctrlr2(3, 2, 4, 5, 6, 7, 11);
// MyControl ctrlr3(3, 2, 4, 5, 6, 7 ,12);

MyControl controllers[1] = {ctrlr1};
// MyControl controllers[3] = {ctrlr1, ctrlr2, ctrlr3};


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
      // - channel attribution for each controller (0x10 + channel number
      // in hex)
      // - controller activation (0x20)
      // - controller desactivation (0x30)
      if (value == 0x10) {
        int newChannel = findUnusedController();
        if (newChannel != -1) {
          MIDI.sendControlChange(20, newChannel - 1, newChannel);  // ControlNumber, ControlValue, Channel
        } else {
          // envoyer un message pour dire qu'il n'y a plus de controller libre
          // TODO in JS
          MIDI.sendControlChange(20, 0, channel);  // ControlNumber, ControlValue, Channel
        }
      } else if (value == 0x20) {
        // Activation
        // 1) find the message's recipient (his own channel  = param
        // 'channel')
        int idx = findByChannelNumber(channel);
        Serial.print("controlleur index: ");
        Serial.println(idx);
        MyControl ctrl = controllers[idx];
        // 2) ask him to activate itself, and to reply with a
        // confirmation message
        ctrl.activate();
      } else if (value == 0x30) {
        // Desactivation
        // update channels array
        // 1) find the message's recipient (his own channel  = param
        // 'channel')
        // TODO
        //        int idx = findByChannelNumber(channel);
      } else if (value == 0x40) {
        // pseudo Active Sensing message
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

  MIDI.begin();  // Listen to all incoming messages
  MIDI.turnThruOff();

  MIDI.setHandleControlChange(OnControlChange);

  Serial.println(F("Arduino ready!"));

  // ctrlr1.activate();
}

void loop() {
  // MIDI ----------
  MIDI.read();

  ctrlr1.update();

  int incr = ctrlr1.getIncrement();
  if (incr != 0) {
    Serial.print("Incr: ");
    Serial.println(ctrlr1.getIncrement());
  }
  /*
  // Message
  int a = ctrlr1.midiMsg[0];
  int b = ctrlr1.midiMsg[1];
  int c = ctrlr1.midiMsg[2];
  if (a != 0 || b != 0 || c != 0) {
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.println(c);
    int msg[] = { 0, 0, 0 };
    ctrlr1.setMidiMsg(msg);
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

int findUnusedController() {
  // TODO: error handling (no more free channel)
  for (int i = 0; i < 3; i++) {
    if (controllers[i].getActivity() == false) {
      return controllers[i].getChannel();
    }
  }
  // No more free controller
  return -1;
}

int findByChannelNumber(int channel_) {
  for (int i = 0; i < 3; i++) {
    if (controllers[i].getChannel() == channel_) {
      return i;
    }
  }
}