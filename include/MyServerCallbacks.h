#ifndef MyServerCallbacks_H
#define MyServerCallbacks_H

#include <Arduino.h>
#include <BLEServer.h>

class MyServerCallbacks : public BLEServerCallbacks {
public:
    bool deviceConnected;
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);
};

#endif