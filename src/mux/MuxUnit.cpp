#include "mux/MuxUnit.hpp"

#include "Address.hpp"
#include "PCF8575.h"
#include "i2c/I2CBus.hpp"

void MuxUnit::setup() {
    expander = new PCF8575(&I2C_WIRE, I2C_EXPANDER_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN);
    expander->begin();
    for (int i = 0; i < 8; i++) {
        expander->pinMode(i, OUTPUT);
    }
    for (int i = 0; i < 8; i++) {
        setPin(i, i == MUX_BRAKE_BLINK_PIN);
    }
}

void MuxUnit::setPin(int pin, bool enabled) { expander->digitalWrite(pin, enabled ? HIGH : LOW); }

void MuxUnit::setVescMosfetEnabled(bool enabled) { setPin(MUX_VESC_MOSFET_PIN, enabled); }

void MuxUnit::setMotorFanEnabled(bool enabled) { setPin(MUX_MOTOR_FAN_PIN, enabled); }

void MuxUnit::setOverallFanEnabled(bool enabled) { setPin(MUX_OVERALL_FAN_PIN, enabled); }

void MuxUnit::setBrakeLedEnabled(bool enabled) {
    setPin(MUX_BRAKE_BLINK_PIN, !enabled);
    setPin(MUX_BRAKE_CONSTANT_PIN, enabled);
}
