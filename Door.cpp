#include "Door.h"
#include "Arduino.h"

Door::Door(int pin) {
  this->pin = pin;
  this->initialized = false;
  this->open = false;
  this->changed = false;
  this->lastChangedAtMillis = 0;
}

void Door::begin() {
  pinMode(this->pin, INPUT);
}

void Door::read() {
  bool doorPin = digitalRead(this->pin);
  unsigned long currentMillis = millis();
  // debounce
  if (doorPin != this->open && currentMillis - this->lastChangedAtMillis < 50) {
    return;
  }

  if (this->initialized && doorPin != this->open) {
    this->changed = true;
    this->lastChangedAtMillis = currentMillis;
  }

  this->open = doorPin;

  if (!this->initialized) {
    this->initialized = true;
  }
}

bool Door::isOpen() {
  return this->open;
}

bool Door::didChange() {
  bool value = this->changed;
  this->changed = false;
  return value;
}
