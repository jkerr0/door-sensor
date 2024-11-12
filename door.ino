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
#include <ESP8266WiFi.h>

#include "Door.h"
#include "BeepingBuzzer.h"
#include "LED.h"
#include "Stopwatch.h"
#include "NetworkManager.h"
#include "HTTPServer.h"

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
Stopwatch doorOpenStopwatch = Stopwatch();
NetworkManager networkManager = NetworkManager();
WiFiServer wifiServer = WiFiServer(80);
HTTPServer httpServer = HTTPServer(&wifiServer, &door);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {
    // Wait for serial
  }

  Serial.print("Connecting to WiFi, SSID: ");
  networkManager.begin();
  Serial.println(WiFi.SSID());

  Serial.println("Initializing door...");
  door.begin();

  Serial.println("Initializing buzzer...");
  buzzer.begin();
  buzzer.beepOnce();

  Serial.println("Initializing LED...");
  statusLed.begin();
  statusLed.on();
  delay(1000);
  statusLed.off();

  Serial.println("Waiting WiFi connection...");
  networkManager.waitForConnection();

  if (networkManager.isConnected()) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Starting HTTP server...");
    httpServer.begin();
  } else {
    Serial.println("Continuing offline");
  }

  Serial.println("Setup complete");
}

void update() {
  door.read();
  buzzer.update();
  statusLed.update();
}

void loop() {
  // put your main code here, to run repeatedly:
  update();
  const unsigned long ALARM_DELAY_MILLIS = 20 * 1000;
  if (door.didChange()) {
    Serial.print("Door status changed to: ");
    if (door.isOpen()) {
      doorOpenStopwatch.start();
      statusLed.blink();
      Serial.println("open");
    } else {
      buzzer.stopBeeping();
      statusLed.off();
      doorOpenStopwatch.stop();
      doorOpenStopwatch.reset();
      Serial.println("closed");
    }
  }

  if (doorOpenStopwatch.getMillisValue() >= ALARM_DELAY_MILLIS) {
    buzzer.startBeeping();
  }

  if (!networkManager.isConnected()) {
    return;
  }
  httpServer.accept();
}
