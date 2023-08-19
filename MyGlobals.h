#ifndef MyGlobals_h
#define MyGlobals_h

const extern int controlsNumber;  // number of controllers

typedef struct {
  int id;
  bool isUsed;
} AChannel;

extern AChannel A;
extern AChannel B;
extern AChannel C;

extern AChannel theChannels[];

#endif