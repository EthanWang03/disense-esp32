#ifndef MyCharacteristicCallbacks_H
#define MyCharacteristicCallbacks_H

//#include "BLEuuids.h"
#include <Arduino.h>
#include <BLECharacteristic.h>

typedef void (*Callback)();

class AckCallback : public BLECharacteristicCallbacks {
private:
    Callback deepSleepCallback;

public:
    void resetAcks();
    bool allAcknowledgmentsRecevied();
    AckCallback();
    bool ack1;
    bool ack2;
    bool ack3;
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif