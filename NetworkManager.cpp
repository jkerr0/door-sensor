#include "NetworkManager.h"
#include "Arduino.h"
#include "credentials.h"
#include <ESP8266WiFi.h>

void NetworkManager::printStatus() {
  const char* msg;
  switch (this->status) {
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

NetworkManager::NetworkManager() {
  this->status = WL_IDLE_STATUS;
}

void NetworkManager::begin() {
  Serial.println(WIFI_SSID);
  this->status = WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void NetworkManager::waitForConnection() {
  this->status = WiFi.status();
  printStatus();
  unsigned long waitStart = millis();
  while ((this->status == WL_DISCONNECTED || this->status == WL_IDLE_STATUS) && millis() - waitStart <= NetworkManager::WAITING_TIMEOUT_MILLIS) {
    delay(500);
    int newStatus = WiFi.status();
    if (this->status != newStatus) {
      printStatus();
    }
    this->status = newStatus;
  }
}

bool NetworkManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}
