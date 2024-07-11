#include "BLEManager.h"
//#include "BLEuuids.h"
#include "FSR.h"
#include "LED.h"
#include "SensorIDs.h"
#include "byte-array-encoder.h"
#include "spo2.h"
#include "thermistor.h"
#include <Arduino.h>
#include <Wire.h>

#define NUM_THERMISTORS 4
#define NUM_FSR 4
#define NUM_SPO2 1
#define BYTES_PER_SENSOR 5 // For byte array (1 byte for sensor id, 4 bytes for temp value)

const char* THERMISTORS_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
const char* FSR_CHARACTERISTIC_UUID = "f00a075c-948e-4f01-9cb6-7d876cf96683";
const char* SPO2_CHARACTERISTIC_UUID = "9f7d8c4f-b3d4-4d72-8787-8386e5f13195";
const char* ACKNOWLEDGMENT_CHARACTERISTIC_UUID = "1b384bed-4282-41e1-8ef9-466bc94fa5ed";

// Sensors
Thermistor *thermistorArr[NUM_THERMISTORS];
FSR *fsrArr[NUM_FSR];
SPO2 *spo2Arr[NUM_SPO2];

// LEDs
LED *bleLed;

// BLE Objects
BLEManager *bleManager;
BLECharacteristic *thermistorCharacteristic;
BLECharacteristic *fsrCharacteristic;
BLECharacteristic *spo2Characteristic;

// byte arrays
uint8_t thermistorByteArr[NUM_THERMISTORS * BYTES_PER_SENSOR]; // 4 bytes per sensor (1 for id, 4 for temperature value -- int = 4 bytes)
uint8_t fsrByteArr[NUM_FSR * BYTES_PER_SENSOR];
uint8_t spo2ByteArr[NUM_SPO2 * BYTES_PER_SENSOR];

const long bleTransmissionInterval = 2000; // Interval for sending data over BLE (2 seconds)
unsigned long prevMillis = 0;              // Stores last time ble tranmission was executed

unsigned long prevTime;


void setup() {
    Serial.begin(115200);
    Wire.begin();
    Serial.println("ESP32 awake");
    prevTime = millis();

    // Create BLE manager object
    bleManager = new BLEManager("Disense-1");

    // Create thermistor objects
    thermistorArr[0] = new Thermistor(25, 1);
    thermistorArr[1] = new Thermistor(33, 2);
    thermistorArr[2] = new Thermistor(32, 3);
    thermistorArr[3] = new Thermistor(34, 4);

    // Create FSR objects
    fsrArr[0] = new FSR(13, 1);
    fsrArr[1] = new FSR(12, 2);
    fsrArr[2] = new FSR(14, 3);
    fsrArr[3] = new FSR(27, 4);

    // Initialize SPO2 objects
    spo2Arr[0] = new SPO2(1, 4, 5);
    spo2Arr[0]->init(Wire);

    // Create LED objects
    bleLed = new LED(2);

    // Create characteristic objects for sensors (thermistor, force, spo2)
    thermistorCharacteristic = bleManager->createBLECharacteristicForNotify(THERMISTORS_CHARACTERISTIC_UUID);
    fsrCharacteristic = bleManager->createBLECharacteristicForNotify(FSR_CHARACTERISTIC_UUID);
    spo2Characteristic = bleManager->createBLECharacteristicForNotify(SPO2_CHARACTERISTIC_UUID);
    bleManager->createCharacteristicForWrite(ACKNOWLEDGMENT_CHARACTERISTIC_UUID);

    bleManager->startService();
    bleManager->startAdvertising();
}

void readAndEncodeThermistorData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_THERMISTORS; i++) {
        float temp = thermistorArr[i]->readTemperature();
        encodeDataToByteArr(temp, thermistorArr[i]->getId(), &thermistorByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
}

void readAndEncodeFSRData() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_FSR; i++) {
        float force = fsrArr[i]->readForce();
        encodeDataToByteArr(force, fsrArr[i]->getId(), &fsrByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
}

void readAndEncodeSPO2Data() {
    int byteArrIndex = 0;
    for (int i = 0; i < NUM_SPO2; i++) {
        bioData data = spo2Arr[i]->readSensor();
        encodeSPO2DataToByteArr(spo2Arr[i]->getId(), data, &spo2ByteArr[byteArrIndex]);
        byteArrIndex += BYTES_PER_SENSOR;
    }
}

void loop() {
    readAndEncodeSPO2Data();
    if (bleManager->getIsDeviceConnected()) {
        unsigned long currTime = millis();
        bleLed->turnOn();

        readAndEncodeThermistorData();
        readAndEncodeFSRData();
        Serial.println(" ");
        Serial.print("Time: ");
        Serial.println(currTime - prevTime);
        if (currTime - prevTime >= 5000) {
            prevTime = currTime;
            Serial.println("Send BLE");
            thermistorCharacteristic->setValue(thermistorByteArr, sizeof(thermistorByteArr));
            thermistorCharacteristic->notify();
            fsrCharacteristic->setValue(fsrByteArr, sizeof(fsrByteArr));
            fsrCharacteristic->notify();
            spo2Characteristic->setValue(spo2ByteArr, sizeof(spo2ByteArr));
            spo2Characteristic->notify();
        }
        delay(200);
    } else {
        bleLed->turnOff();
        delay(500);
        bleLed->turnOn();
        delay(300);
    }
    // delay(2000);
}

