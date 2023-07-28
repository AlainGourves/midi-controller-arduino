/* Encoder Library 
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 */

// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>


#define SW1 4
#define SW2 5
#define LED1 6
#define LED2 7

int stateSW1 = 1; // HIGH because of the PULL_UP
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
#define MAX_VAL 127
#define MIN_VAL 0

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  pinMode(SW1, INPUT_PULLUP);  // SW1 (encoder)
  pinMode(SW2, INPUT_PULLUP);  // SW2 (momentary switch)
  pinMode(LED1, OUTPUT);       // LED1 (green)
  pinMode(LED2, OUTPUT);       // LED2 (red)
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  Serial.println("Encoder ready!");
}

long oldPosition = -999;

void loop() {
  // Encoder ----------
  long newPosition = myEnc.read() >> 2;  // encoder reads 1 step as 4
  if (newPosition > MAX_VAL) {
    newPosition = MIN_VAL;
    myEnc.write(MIN_VAL);
  }
  if (newPosition < MIN_VAL) {
    newPosition = MAX_VAL;
    myEnc.write(MAX_VAL * 4);
  }
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }

  // Momentary Switches -----------
  int readSW1 = digitalRead(SW1);
  int readSW2 = digitalRead(SW2);

  if (readSW1 != lastStateSW1) {
    lastDebounceTimeSW1 = millis();
  }
  if ((millis() - lastDebounceTimeSW1) > debounceDelay) {
    if (readSW1 != stateSW1) {
      stateSW1 = readSW1;
      if(stateSW1 == LOW) {
      stateLED1 = !stateLED1;
      }
    }
  }

  if (readSW2 != lastStateSW2) {
    lastDebounceTimeSW2 = millis();
  }
  if ((millis() - lastDebounceTimeSW2) > debounceDelay) {
    if (readSW2 != stateSW2) {
      stateSW2 = readSW2;
      if(stateSW2 == LOW) {
      stateLED2 = !stateLED2;
      }
    }
  }


  digitalWrite(LED1, stateLED1);
  digitalWrite(LED2, stateLED2);
  lastStateSW1 = readSW1;
  lastStateSW2 = readSW2;
}
