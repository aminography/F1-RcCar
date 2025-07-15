#include "MuxUnit.hpp"

#include "Address.hpp"
#include "PCF8575.h"

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

void MuxUnit::setPin(const int pin, const bool enabled) const {
    expander->digitalWrite(pin, enabled ? HIGH : LOW);
}

void MuxUnit::setDrsEnabled(bool enabled) const {
    setPin(MUX_DRS_PIN, enabled);
}

void MuxUnit::setVescMosfetEnabled(const bool enabled) const {
    setPin(MUX_VESC_MOSFET_PIN, enabled);
}

void MuxUnit::setMotorFanEnabled(const bool enabled) const {
    setPin(MUX_MOTOR_FAN_PIN, enabled);
}

void MuxUnit::setOverallFanEnabled(const bool enabled) const {
    setPin(MUX_OVERALL_FAN_PIN, enabled);
}

void MuxUnit::setBrakeLedEnabled(const bool enabled) const {
    setPin(MUX_BRAKE_BLINK_PIN, !enabled);
    setPin(MUX_BRAKE_CONSTANT_PIN, enabled);
}
