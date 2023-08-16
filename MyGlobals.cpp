#include <Arduino.h>
#include "MyGlobals.h"

const int controlsNumber = 3;


struct AChannel A = { 10, false };
struct AChannel B = { 11, false };
struct AChannel C = { 12, false };

struct AChannel theChannels[controlsNumber] = {A, B, C};
