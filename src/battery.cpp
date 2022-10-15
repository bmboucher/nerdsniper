#include "nerdsniper.h"

#define BATTERY_READ_TIME_MS 500

float battery_level = 0.0;

namespace {
    const float LAMBDA = 0.9;
    unsigned long last_battery_read = 0;

    float singleRead() {
        // ADC scale is 3.3V over 1024 (2^10) bits
        // ADC on pin 29 (VSYS_ADC) is connected to VSYS by 1/3 voltage divider
        float read = analogRead(VSYS_ADC) * 3.3f * 3 / (1 << 10);
        last_battery_read = millis();
        return read;
    }

    void serialPrintBattery() {
        Serial.print(F("Battery: "));
        Serial.print(battery_level);
        Serial.println('V');
    }
}

void initBattery() {
    pinMode(VSYS_ADC, INPUT);
    battery_level = singleRead();
    serialPrintBattery();
}

void readBattery() {
    unsigned long curr_t = millis();
    if (curr_t < last_battery_read || curr_t >= last_battery_read + BATTERY_READ_TIME_MS) {
        const float new_read = singleRead();
        battery_level = LAMBDA * battery_level + (1 - LAMBDA) * new_read;
        serialPrintBattery();
    }
}