#ifndef SPO2_H
#define SPO2_H

#include <Arduino.h>
#include <SparkFun_Bio_Sensor_Hub_Library.h>

class SPO2 {
private:
    SparkFun_Bio_Sensor_Hub bioHub;
    bioData body;
    int resPin;
    int mfioPin;
    int id;
    void startSensor(TwoWire &wirePort);
    void configureSensor();

public:
    SPO2(int id, int reset, int mfio);
    void init(TwoWire &wire);
    bioData readSensor();
    int getId();
    int status;
    int oxygen;
};

#endif