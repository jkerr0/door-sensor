#ifndef BeepingBuzzer_h
#define BeepingBuzzer_h

#include "Arduino.h"

/**
 * Passive buzzer
 **/
class BeepingBuzzer {
private:
  int pin;
  bool shouldBeep;
  unsigned long interval;
  unsigned long duration;
  unsigned int frequency;
  unsigned long lastBeepStartMillis;
  unsigned long lastBeepEndMillis;
public:
  BeepingBuzzer(int pin);
  void startBeeping();
  void stopBeeping();
  void beepOnce();
  void begin();
  void update();
};

#endif