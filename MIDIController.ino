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

#define SW1 4
#define SW2 5
#define LED1 6
#define LED2 7

int stateSW1 = 1;  // HIGH because of the PULL_UP
int stateSW2 = 1;
int stateLED1 = 0;
int stateLED2 = 0;
int lastStateSW1 = 1;
int lastStateSW2 = 1;
unsigned long lastDebounceTimeSW1 = 0;
unsigned long lastDebounceTimeSW2 = 0;
unsigned long debounceDelay = 50;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 2);  // INT0 & INT 1

static void OnControlChange(byte channel, byte number, byte value) {
  Serial.print(F("ControlChange from channel: "));
  Serial.print(channel);
  Serial.print(F(", number: "));
  Serial.print(number);
  Serial.print(F(", value: "));
  Serial.println(value);
  switch (number) {
    case 20:
      // active/désactive le bazar
      isControllerActive = !isControllerActive;
      if (stateLED2) {
        // turn off red led 
        stateLED2 = 0;
      }
      break;
    default:
      Serial.println("message reçu");
      break;
  }
}

void setup() {
  Serial.begin(31250); // MIDI baud rate
  while (!Serial)
    ;

  pinMode(SW1, INPUT_PULLUP);  // SW1 (encoder)
  pinMode(SW2, INPUT_PULLUP);  // SW2 (momentary switch)
  pinMode(LED1, OUTPUT);       // LED1 (green)
  pinMode(LED2, OUTPUT);       // LED2 (red)
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  MIDI.begin(1);
  MIDI.turnThruOff();
  MIDI.setHandleControlChange(OnControlChange);

  Serial.println("Arduino ready!");
}

long oldPosition = 0;

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
      if (diff > 0 && stateLED2) val = 10;
      if (diff < 0 && stateLED2) val = 30;
      oldPosition = newPosition;
      Serial.println(newPosition);
      // Send MIDI message
      MIDI.sendControlChange(21, val, 1);
    }
  }

  // Momentary Switches -----------
  int readSW1 = digitalRead(SW1);  // Encoder's Switch
  int readSW2 = digitalRead(SW2);  // Momentary Switch

  if (readSW1 != lastStateSW1) {
    lastDebounceTimeSW1 = millis();
  }
  if ((millis() - lastDebounceTimeSW1) > debounceDelay) {
    if (readSW1 != stateSW1) {
      Serial.println(readSW1);
      stateSW1 = readSW1;
      if (stateSW1 == LOW) {
        Serial.println("reinit value");
      }
    }
  }

  if (readSW2 != lastStateSW2) {
    lastDebounceTimeSW2 = millis();
  }
  if ((millis() - lastDebounceTimeSW2) > debounceDelay) {
    if (readSW2 != stateSW2 && isControllerActive) {
      stateSW2 = readSW2;
      if (stateSW2 == LOW) {
        stateLED2 = !stateLED2;
      }
    }
  }


  digitalWrite(LED1, isControllerActive);
  digitalWrite(LED2, stateLED2);
  lastStateSW1 = readSW1;
  lastStateSW2 = readSW2;
}
