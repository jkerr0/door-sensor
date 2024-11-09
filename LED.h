#ifndef LED_h
#define LED_h

#include "Arduino.h"

class LED {
  private:
    int pin;
    uint8_t state;
    bool shouldBlink;
    unsigned long blinkDuration;
    unsigned long blinkInterval;
    unsigned long lastBlinkStart;
    unsigned long lastBlinkEnd;
    void write();
    void blinkReset();
  public:
    LED(int pin);
    void on();
    void off();
    void blink();
    void blink(unsigned long duration);
    void blink(unsigned long duration, unsigned long interval);
    void begin();
    void update();
};

#endif