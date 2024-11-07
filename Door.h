#ifndef Door_h
#define Door_h

#include "Arduino.h"

class Door {
  private:
    int pin;
    bool open;
    bool initialized;
    bool changed;
    unsigned long lastChangedAtMillis;
  public:
    void begin();
    Door(int pin);
    void read();
    bool isOpen();
    bool didChange();
};

#endif