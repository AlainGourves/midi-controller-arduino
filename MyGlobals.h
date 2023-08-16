#ifndef MyGlobals_h
#define MyGlobals_h

const extern int controlsNumber;  // number of controllers

struct AChannel {
  int id;
  bool isUsed;
};

extern struct AChannel A;
extern struct AChannel B;
extern struct AChannel C;

extern struct AChannel theChannels[];


#endif