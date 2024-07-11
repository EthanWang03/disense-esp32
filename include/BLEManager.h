#ifndef BLEManager_H
#define BLEManager_H

//#include "BLEuuids.h"
#include "MyServerCallbacks.h"
#include "ack-callback.h"
#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

typedef void (*DeepSleepCallback)();

class BLEManager {
private:
    BLEServer *server;
    BLEService *service;
    void createBLEServer();
    void createBLEService();

public:
    MyServerCallbacks *serverCallbacks;
    BLEManager(std::string deviceName);
    AckCallback *ackCallback;
    void startService();
    void startAdvertising();
    BLECharacteristic *createBLECharacteristicForNotify(const char *characteristicUuid);
    BLECharacteristic *createCharacteristicForWrite(const char *characteristicUuid);
    bool getIsDeviceConnected();
    void bleShutDown();
};

#endif