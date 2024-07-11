#include "FSR.h"

/**
 * Connection steps:
 *  https://www.geeksforgeeks.org/force-sensitive-resistor-fsr-with-arduino/
 */

FSR::FSR(byte pin, int id) {
    this->pin = pin;
    this->id = id;
}

int FSR::getId() {
    return this->id;
}

float FSR::readForce() {
    int fsrADC = analogRead(this->pin);
    float voltage = this->fromADCValToVoltage(fsrADC);
    float resistance = this->fromVoltageToResistance(voltage);
    float force = this->fromResistanceToNewtonsUsing1kResistor(resistance);

    Serial.print("FSR force ");
    if (this->id == 1) {
        Serial.print("Metatarsal 1");
    } else if (this->id == 2) {
        Serial.print("Metatarsal 5");
    } else if (this->id == 3) {
        Serial.print("Heel");
    } else if (this->id == 4) {
        Serial.print("Big toe");
    }
    Serial.print(" : ");
    Serial.println(force);

    return round(force * 10) / 10;
}

float FSR::fromADCValToVoltage(float adcVal) {
    return (float(adcVal) / float(4095)) * 3.3;
}

float FSR::fromVoltageToResistance(float voltage) {
    return 1000.0 * ((3.3 - voltage) / voltage);
}

/**
 * @brief Converts voltage to newtons using calibration curve (using 20kOhm resistor)
 * @param [in] voltage - The voltage of the fsr sensor
 * @return The force in newtons
 */
float FSR::fromVoltageToNewtonsUsing20KResistor(float voltage) {
    /**
     * Using calibration curve equation: V = 0.9152ln(F) - 2.5498
     * (V = voltage, F = force in newtons)
     *
     * Solve for V: F=e^(1.09265734265734V)*16.2169613641861
     */
    return exp(1.09265734265734 * voltage) * 16.2169613641861;
}

/**
 * @brief Converts voltage to newtons using calibration curve (using 10kOhm resistor)
 * @param [in] voltage - The voltage of the fsr sensor
 * @return The force in newtons
 */
float FSR::fromResistanceToNewtonsUsing10kResistor(float resistance) {
    /**
     * Using calibration curve equation: V = 1.3714ln(F) - 6.4288
     * (V = voltage, F = force in newtons)
     *
     * Solve for V: F=e^(0.729181857955374V)*108.610091681168
     *
     * y=6055.1x^-0.399
     */
    // if (resistance > 38000) {
    //     return 0.0;
    // }

    // return 6055.1 * pow(resistance, -0.399);
    return 31912 - 30634.6 * pow(resistance, 0.0038987);
}

float FSR::fromResistanceToNewtonsUsing1kResistor(float resistance) {
    if (resistance > 300000) {
        return 0;
    }
    float force = 7771.06 - 6435.79 * pow(resistance, 0.0127751);
    return force;
}
