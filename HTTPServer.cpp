#include "HTTPServer.h"
#include "Door.h"

HTTPServer::HTTPServer(WiFiServer* server, Door* door) {
  this->server = server;
  this->door = door;
}

void HTTPServer::begin() {
  server->begin();
}

void HTTPServer::accept() {
  WiFiClient client = server->available();
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
  client.print(door->isOpen() ? "true" : "false");
  client.println(" }");
  client.flush();
}