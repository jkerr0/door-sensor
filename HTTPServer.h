#ifndef HTTPServer_h
#define HTTPServer_h

#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "Door.h"

class HTTPServer {
  private:
    WiFiServer* server;
    Door* door;
  public:
    HTTPServer(WiFiServer* server, Door* door);
    void accept();
    void begin();
};

#endif