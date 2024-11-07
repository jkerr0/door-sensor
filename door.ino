/**
    Copyright 2024 Jan Rokicki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**/

#include "Door.h"

// podłączamy kontaktron tak:
// jedna nóżka na GND
// dróga nóżka przez pullup 10K do 5V i to do pinu czytanego
Door door = Door(12);

void setup() {
  // put your setup code here, to run once:
  door.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  door.read();
  if (door.didChange()) {
    Serial.print("Changed to ");
    if (door.isOpen()) {
      Serial.println("open");
    } else {
      Serial.println("closed");
    }
  }
}
