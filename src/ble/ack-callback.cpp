#include "ack-callback.h"

AckCallback::AckCallback() {
    this->deepSleepCallback = deepSleepCallback;
    this->resetAcks();
}

void AckCallback::onWrite(BLECharacteristic *pCharacteristic) {
    std::string val = pCharacteristic->getValue();

    if (val == "thermistor") {
        this->ack1 = true;
    } else if (val == "fsr") {
        this->ack2 = true;
    } else if (val == "spo2") {
        this->ack3 = true;
    }

    Serial.println("Acknowledgement vals: ");
    Serial.print("Thermistor ack: ");
    Serial.println(this->ack1);
    Serial.print("FSR ack: ");
    Serial.println(this->ack2);
    Serial.print("SPO2 ack: ");
    Serial.println(this->ack3);
    Serial.println(" ");

    // if (this->ack1 && this->ack2) {
    //     this->resetAcks();
    //     this->deepSleepCallback();
    // }
}

void AckCallback::resetAcks() {
    this->ack1 = false;
    this->ack2 = false;
    this->ack3 = false;
}

bool AckCallback::allAcknowledgmentsRecevied() {
    return this->ack1 && this->ack2 && this->ack3;
}