#include "LED.h"

LED::LED(byte pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}

void LED::turnOn() {
    digitalWrite(this->pin, HIGH);
}

void LED::turnOff() {
    digitalWrite(this->pin, LOW);
}