#include "i2c/I2CBus.hpp"

#include <Arduino.h>
#include <Wire.h>

#include "Address.hpp"

/* Wire as the I2C-0 has been used by the displays on pins 17 & 18, not possible to change.
So, this wrapper provides Wire1 as the I2C interface for the rest of devices. */

void scan(TwoWire& bus, const char* busName);

void I2CBus::setup() { I2C_WIRE.begin(I2C_SDA_PIN, I2C_SCL_PIN); }

void I2CBus::busScan() { scan(I2C_WIRE, "Wire1 I2C"); }

void scan(TwoWire& bus, const char* busName) {
    Serial.printf("Scanning %s...\n", busName);
    uint8_t count = 0;

    for (uint8_t address = 1; address < 127; address++) {
        bus.beginTransmission(address);
        if (bus.endTransmission() == 0) {
            Serial.printf("  Found device at 0x%02X\n", address);
            count++;
        }
        delay(2);
    }

    if (count == 0) {
        Serial.printf("  No I2C devices found on %s\n", busName);
    } else {
        Serial.printf("  Done. %d device(s) found on %s\n", count, busName);
    }
    Serial.println();
}
