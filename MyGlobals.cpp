#include <Arduino.h>
#include "MyGlobals.h"

const int controlsNumber = 3;


AChannel A = { 10, false };
AChannel B = { 11, false };
AChannel C = { 12, false };

AChannel theChannels[controlsNumber] = {A, B, C};
