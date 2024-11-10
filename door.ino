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
#include "credentials.h"

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
Stopwatch alarmStopwatch = Stopwatch();
WiFiServer server = WiFiServer(80);

void printWifiStatus() {
  const char* msg;
  switch (WiFi.status()) {
    case WL_CONNECTED:
      msg = "Connected";
      break;
    case WL_NO_SHIELD:
      msg = "No WiFi shield";
      break;
    case WL_IDLE_STATUS:
      msg = "Idle";
      break;
    case WL_CONNECT_FAILED:
      msg = "Connection failed";
      break;
    case WL_NO_SSID_AVAIL:
      msg = "No SSID available";
      break;
    case WL_SCAN_COMPLETED:
      msg = "Network scan completed";
      break;
    case WL_DISCONNECTED:
      msg = "Disconnected";
      break;
    case WL_CONNECTION_LOST:
      msg = "Connection lost";
      break;
  }

  Serial.print("WiFi status: ");
  Serial.println(msg);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {
    // Wait for serial
  }

  Serial.print("Connecting to WiFi, SSID: ");
  Serial.println(WIFI_SSID);
  int wifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASS);

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

  Serial.println("Awaiting WiFi connection...");
  wifiStatus = WiFi.status();
  printWifiStatus();
  while (wifiStatus == WL_DISCONNECTED) {
    delay(500);
    int newStatus = WiFi.status();
    if (wifiStatus != newStatus) {
      printWifiStatus();
    }
    wifiStatus = newStatus;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Starting server...");
    server.begin();
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
      alarmStopwatch.start();
      statusLed.blink();
      Serial.println("open");
    } else {
      buzzer.stopBeeping();
      statusLed.off();
      alarmStopwatch.stop();
      alarmStopwatch.reset();
      Serial.println("closed");
    }
  }

  if (alarmStopwatch.getMillisValue() >= ALARM_DELAY_MILLIS) {
    buzzer.startBeeping();
  }

  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  int headerCounter = 0;
  while (client.connected()) {
    if (client.available()) {
      String header = client.readStringUntil('\n');
      if (headerCounter == 0) {
        Serial.print("Received request at: ");
        Serial.println(header);
      }
      if (header.equals("\r")) {
        break;
      }
      headerCounter++;
    }
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print("{ \"doorOpen\": ");
  client.print(door.isOpen() ? "true" : "false");
  client.println(" }");
  client.flush();
}
