#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
private:
    byte pin;

public:
    LED(byte pin);
    void turnOn();
    void turnOff();
};

#endif