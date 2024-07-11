#include "byte-array-encoder.h"

void encodeDataToByteArr(float val, int id, uint8_t *byteArr) {
    int valInteger = val * 10; // multiply by 10 to move decimal to the right
    byteArr[0] = id;
    byteArr[1] = (valInteger >> 24) & 0xFF;
    byteArr[2] = (valInteger >> 16) & 0xFF;
    byteArr[3] = (valInteger >> 8) & 0xFF;
    byteArr[4] = (valInteger) & 0xFF;
}

void encodeSPO2DataToByteArr(int id, bioData data, uint8_t *byteArr) {
    // id, heartrate, blood oxygen
    // id = 1 byte, heartrate = 2 bytes (max is 220), blood oxygen = 2 bytes (max is 100)
    byteArr[0] = id;
    byteArr[1] = (data.heartRate >> 8) & 0xFF;
    byteArr[2] = data.heartRate & 0xFF;
    byteArr[3] = (data.oxygen >> 8) & 0xFF;
    byteArr[4] = data.oxygen & 0xFF;
}