#include "Stopwatch.h"
#include "Arduino.h"

Stopwatch::Stopwatch() {
  reset();
}

void Stopwatch::reset() {
  this->startedAt = Stopwatch::NOT_STARTED;
  this->value = 0;
}

void Stopwatch::start() {
  if (this->startedAt == Stopwatch::NOT_STARTED) {
    this->startedAt = millis();
  }
}

void Stopwatch::stop() {
  this->value += millis() - this->startedAt;
  this->startedAt = Stopwatch::NOT_STARTED;
}

unsigned long Stopwatch::getMillisValue() {
  if (this->startedAt == Stopwatch::NOT_STARTED) {
    return this->value;
  } else {
    return this->value + millis() - this->startedAt;
  }
}