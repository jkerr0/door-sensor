#ifndef NetworkManager_h
#define NetworkManager_h

#include "Arduino.h"

class NetworkManager {
  private:
    static const unsigned long WAITING_TIMEOUT_MILLIS = 30 * 1000;
    int status;
  public:
    NetworkManager();
    void begin();
    void waitForConnection();
    bool isConnected();
    void printStatus();
};

#endif