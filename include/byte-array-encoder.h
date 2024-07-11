#include "SparkFun_Bio_Sensor_Hub_Library.h"
#include <Arduino.h>

void encodeDataToByteArr(float val, int id, uint8_t *byteArr);
void encodeSPO2DataToByteArr(int id, bioData data, uint8_t *byteArr);