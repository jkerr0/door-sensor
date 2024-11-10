#include "LED.h"
#include "Arduino.h"

LED::LED(int pin) {
  this->pin = pin;
  this->state = LOW;
  this->blinkDuration = 100;
  this->blinkInterval = 100;
  this->lastBlinkEnd = 0;
  this->lastBlinkStart = 0;
  this->shouldBlink = false;
}

void LED::begin() {
  pinMode(this->pin, OUTPUT);
  this->state = HIGH;
  write();
  delay(1000);
  this->state = LOW;
  write();
}

void LED::blink() {
  this->shouldBlink = true;
  this->blinkDuration = 100;
  this->blinkInterval = 100;
}

void LED::blink(unsigned long duration) {
  this->shouldBlink = true;
  this->blinkDuration = duration;
  this->blinkInterval = duration;
}

void LED::blink(unsigned long duration, unsigned long interval) {
  this->shouldBlink = true;
  this->blinkDuration = duration;
  this->blinkInterval = interval;
}

void LED::blinkReset() {
  this->lastBlinkStart = millis();
  this->lastBlinkEnd = this->lastBlinkStart + this->blinkDuration;
}

void LED::on() {
  this->shouldBlink = false;
  this->state = HIGH;
  write();
}

void LED::off() {
  this->shouldBlink = false;
  this->state = LOW;
  write();
}

void LED::write() {
  digitalWrite(this->pin, this->state);
}

void LED::update() {
  if (!this->shouldBlink) {
    return;
  }

  unsigned long currentMillis = millis();
  unsigned long start = this->lastBlinkStart;
  unsigned long end = this->lastBlinkEnd;
  if (start <= currentMillis && currentMillis < end && this->state == LOW) {
    this->state = HIGH;
    write();
  } else if (end <= currentMillis && currentMillis < end + this->blinkInterval && this->state == HIGH) {
    this->state = LOW;
    write();
  } else if (currentMillis < start || currentMillis > end + this->blinkInterval) {
    blinkReset();
  }
}