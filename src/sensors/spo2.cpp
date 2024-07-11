#include "spo2.h"

SPO2::SPO2(int id, int reset, int mfio) {
    this->mfioPin = mfio;
    this->resPin = reset;
    this->id = id;
    this->status = 0;
    this->oxygen = 0;
}

int SPO2::getId() {
    return this->id;
}

void SPO2::init(TwoWire &wire) {
    this->startSensor(wire);
    this->configureSensor();

    // Possible widths: 69, 118, 215,
    int width = 411;
    // Possible samples: 50, 100, 200, 400, 800, 1000, 1600, 3200 samples/second
    // Not every sample amount is possible with every width; check out our hookup
    // guide for more information.
    int samples = 400;
    int pulseWidthVal;
    int sampleVal;

    // Set pulse width.
    // Higher pulse width = longer light emission duration
    // light can penetrate deeper into the skin
    int error = bioHub.setPulseWidth(width);
    if (error == 0) { // Zero errors.
        Serial.println("Pulse Width Set.");
    } else {
        Serial.println("Could not set Pulse Width.");
        Serial.print("Error: ");
        Serial.println(error);
    }

    // Check that the pulse width was set.
    pulseWidthVal = bioHub.readPulseWidth();
    Serial.print("Pulse Width: ");
    Serial.println(pulseWidthVal);

    Serial.println("Loading up the buffer with data....");
    // delay(1000);
}

bioData SPO2::readSensor() {
    this->body = bioHub.readBpm();
    Serial.print("Heart Rate: ");
    Serial.println(body.heartRate);
    Serial.print("Oxygen: ");
    Serial.println(body.oxygen);
    Serial.print("Status: ");
    Serial.println(body.status);
    this->status = body.status;
    this->oxygen = body.oxygen;
    return body;
}

/*PRIVATE METHODS*/

void SPO2::startSensor(TwoWire &wire) {
    int result = this->bioHub.begin(wire, this->resPin, this->mfioPin);
    if (result == 0) { // Zero errors!
        Serial.println("Sensor started!");
    } else {
        Serial.print("Could not communicate with the sensor: ");
        Serial.println(result);
    }
}

void SPO2::configureSensor() {
    Serial.println("Configuring Sensor....");
    int error = this->bioHub.configBpm(MODE_ONE); // Configuring just the BPM settings.

    if (error == 0) { // Zero errors!
        Serial.println("Sensor configured.");
    } else {
        Serial.println("Error configuring sensor.");
        Serial.print("Error: ");
        Serial.println(error);
    }
}