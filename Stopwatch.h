#ifndef Timer_h
#define Timer_h

#include "Arduino.h"

class Stopwatch {
private:
  unsigned long startedAt;
  unsigned long value;
  static const unsigned long NOT_STARTED = 0;
public:
  Stopwatch();
  void start();
  void stop();
  void reset();
  unsigned long getMillisValue();
};

#endif