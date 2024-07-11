#include "MyServerCallbacks.h"

void MyServerCallbacks::onConnect(BLEServer *pServer) {
    this->deviceConnected = true;
};

void MyServerCallbacks::onDisconnect(BLEServer *pServer) {
    this->deviceConnected = false;
    pServer->startAdvertising();
}