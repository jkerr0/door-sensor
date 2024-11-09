#include "BeepingBuzzer.h"
#include "Arduino.h"

BeepingBuzzer::BeepingBuzzer(int pin) {
  this->pin = pin;
  this->shouldBeep = false;
  this->interval = 2000;
  this->duration = 200;
  this->frequency = 600;
  this->lastBeepStartMillis = 0;
  this->lastBeepEndMillis = 0;
}

void BeepingBuzzer::startBeeping() {
  this->shouldBeep = true;
}

void BeepingBuzzer::stopBeeping() {
  this->shouldBeep = false;
}

void BeepingBuzzer::begin() {
  pinMode(this->pin, OUTPUT);
  tone(this->pin, this->frequency, 1000);
}

void BeepingBuzzer::update() {
  if (!this->shouldBeep) {
    return;
  }
  unsigned long currentMillis = millis();
  unsigned long start = this->lastBeepStartMillis;
  unsigned long end = this->lastBeepEndMillis;
  if (start <= currentMillis && currentMillis <= end + this->interval) {
    return;
  }
  tone(this->pin, this->frequency, this->duration);
  this->lastBeepStartMillis = currentMillis;
  this->lastBeepEndMillis = this->lastBeepStartMillis + this->duration;
}