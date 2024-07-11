#include "thermistor.h"

// Connection steps: https://learn.adafruit.com/thermistor/using-a-thermistor

Thermistor::Thermistor(byte pin, const int id) {
    this->pin = pin;
    this->id = id;
}

int Thermistor::getId() {
    return this->id;
}

/**
 * @brief Reads voltage value from analog pin and converts it to a temperature value (in celcius).
 * @return The temperature of the thermistor in celcius.
 */
float Thermistor::readTemperature() {
    int adcVoltage = this->readFromADC();
    float thermistorVoltage = this->fromADCReadingToVoltage(float(adcVoltage));
    float thermistorResistance = this->fromVoltageToResistance(thermistorVoltage);
    float celsiusValue = this->fromResistanceToCelsiusUsingLookupTable(thermistorResistance);

    Serial.print("Thermistor temp ");
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
    Serial.println(celsiusValue);
    // round to nearest 10th decimal place
    celsiusValue = round(celsiusValue * 10) / 10;

    return celsiusValue;
}

/**
 * @brief Reads from ADC pin to get adc reading
 * @return The voltage reading as an ADC reading (0-4095).
 */
int Thermistor::readFromADC() {
    return analogRead(this->pin);
}

/**
 * @brief Converts the adc voltage read from analog pin to actual voltage of thermistor
 * @param [in] adcVoltage - The voltage value read from adc
 * @return The actual voltage value of the thermistor
 */
float Thermistor::fromADCReadingToVoltage(float adcVoltage) {
    int adcMaxVal = 4095; // For 12-bit ADC, max value is 4095
    return (float(adcVoltage) / float(adcMaxVal)) * this->supplyVoltage;
}

/**
 * @brief Converts the thermistor voltage to resistance
 * @param [in] voltage - The voltage read from the analog pin
 * @return The resistance of the thermistor.
 */
float Thermistor::fromVoltageToResistance(float voltage) {
    // Using voltage divider formula
    float r_fixed = 10000.0; // resistor used in voltage divider circuit
    float resistance = r_fixed * (voltage / (this->supplyVoltage - voltage));
    return resistance;
}

/**
 * @brief Converts the thermistor resistance to temperature in celsius
 * @param [in] resistance - The restiance of the thermistor
 * @return The temperature in celsius.
 */
float Thermistor::fromResistanceToCelsius(float resistance) {
    // Using steinhart-hart equation
    float T0 = 298.15; // reference temperature in Kelvin (25C = 298.15K)
    float B = 3950;    // b parameter found in datasheet of thermistor
    float R0 = 10000;  // thermistor resistance at T0 (usually 10k ohms)

    // Get kelvin temp value
    float tempInKelvin = 1 / (0.001417214376 + 0.0001863455008 * log(resistance) + (0.0000003447468826 * pow(log(resistance), 3)));

    // convert kelvin to celsius
    return tempInKelvin - 273.15;
}

/**
 * @brief Gets temperature from lookup table using the resistance value
 * @param [in] resistance - The restiance of the thermistor
 * @return The temperature in celsius.
 */
float Thermistor::fromResistanceToCelsiusUsingLookupTable(float resistance) {
    int leftIndex = 0;
    int rightIndex = lookupTableSize - 1;
    resistance = resistance / 1000.00; // convert Ohm to kOhm;

    // Using binary search
    while (leftIndex <= rightIndex) {
        int m = leftIndex + (rightIndex - leftIndex) / 2; // middle index
        if (resistance == thermistorLookupTable[m].res) {
            return thermistorLookupTable[m].temp;
        }

        if (resistance < thermistorLookupTable[m].res) {
            if (m == lookupTableSize - 1) { // reached end of array
                return 9999.99;             // TODO: What to return if value is greater than last value in array?
            }

            if (resistance >= thermistorLookupTable[m + 1].res) {

                return linearInterpolation(resistance, thermistorLookupTable[m], thermistorLookupTable[m + 1]);
            } else {
                leftIndex = m + 1;
            }
        } else if (resistance > thermistorLookupTable[m].res) {
            if (m == 0) {        // reached beginning of array
                return -9999.99; // TODO: What to return if value is less than first value in array?
            }
            if (resistance <= thermistorLookupTable[m - 1].res) {
                return linearInterpolation(resistance, thermistorLookupTable[m], thermistorLookupTable[m - 1]);
            } else {
                rightIndex = m - 1;
            }
        }
    }

    return 99999;
}

/**
 * @brief Does linear interpolation to get a more accurate temperature value
 * @param [in] resistance - The restiance of the thermistor
 * @param [in] entry1 - entry from thermistorLookupTable (x0, y0)
 * @param [in] entry2 - entry from thermistorLookupTable (x1, y1)
 * @return The temperature using linear interpolation
 */
float Thermistor::linearInterpolation(float resistance, ThermistorTableEntry entry1, ThermistorTableEntry entry2) {
    float temp = entry1.temp + (resistance - entry1.res) * ((entry2.temp - entry1.temp) / (entry2.res - entry1.res));
    return temp;
}