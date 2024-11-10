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
#include "BeepingBuzzer.h"
#include "LED.h"

#define D4 2
#define D3 0
#define D2 4

// podłączamy kontaktron tak:
// jedna nóżka na GND
// dróga nóżka przez pullup 10K do 5V i to do pinu czytanego
Door door = Door(D2);
// PWM, passive buzzer
BeepingBuzzer buzzer = BeepingBuzzer(D3);
LED statusLed = LED(D4);

void setup() {
  // put your setup code here, to run once:
  door.begin();
  buzzer.begin();
  statusLed.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  door.read();
  buzzer.update();
  statusLed.update();
  if (door.didChange()) {
    Serial.print("Changed to ");
    if (door.isOpen()) {
      buzzer.startBeeping(); 
      statusLed.blink();
      Serial.println("open");
    } else {
      buzzer.stopBeeping();
      statusLed.off();
      Serial.println("closed");
    }
  }
}
