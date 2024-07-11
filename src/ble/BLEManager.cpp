#include "BLEManager.h"

const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* characteristicUuid = "0d27736f-aa5b-4c12-adc6-6a1927d298d8";

/**
 * @brief Initialize BLEManager and create the device, server and service
 * @param [in] deviceName - The name of the BLE device
 */
BLEManager::BLEManager(std::string deviceName) {
    BLEDevice::init(deviceName);
    this->serverCallbacks = new MyServerCallbacks();
    this->ackCallback = new AckCallback();
    this->createBLEServer();
    this->createBLEService();
}

/**
 * @brief Creates instance of BLE server
 */
void BLEManager::createBLEServer() {
    this->server = BLEDevice::createServer();
    this->server->setCallbacks(this->serverCallbacks);
}

/**
 * @brief Creates instance of a BLE service
 */
void BLEManager::createBLEService() {
    this->service = this->server->createService(SERVICE_UUID);
}

/**
 * @brief Creates instance of a BLE characteristic that will be used for notification
 * @param [in] characteristicUuid - uuid of the characteristic
 * @return Instance of the BLE Characteristic
 */
BLECharacteristic *BLEManager::createBLECharacteristicForNotify(const char *characteristicUuid) {
    BLECharacteristic *characteristic = this->service->createCharacteristic(
        characteristicUuid,
        BLECharacteristic::PROPERTY_NOTIFY);

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    characteristic->addDescriptor(new BLE2902());

    return characteristic;
}

BLECharacteristic *BLEManager::createCharacteristicForWrite(const char *characteristicUuid) {
    BLECharacteristic *characteristic = this->service->createCharacteristic(
        characteristicUuid,
        BLECharacteristic::PROPERTY_WRITE);
    characteristic->setCallbacks(this->ackCallback);

    return characteristic;
}

/**
 * @brief Starts the BLE service
 */
void BLEManager::startService() {
    this->service->start();
}

/**
 * @brief Starts the BLE advertising to expose it to central devices (smartphone)
 */
void BLEManager::startAdvertising() {
    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(false);
    advertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

/**
 * @brief Returns if the device is connected to a central device
 * @return The variable containing if the device is connected or not
 */
bool BLEManager::getIsDeviceConnected() {
    return this->serverCallbacks->deviceConnected;
}

void BLEManager::bleShutDown() {
    BLEDevice::deinit();
}