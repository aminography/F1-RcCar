#include "ExpanderUnit.hpp"

#include "Address.hpp"
#include "PCF8575.h"

void ExpanderUnit::setup() {
    expander = new PCF8575(&I2C_WIRE, I2C_EXPANDER_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN);
    expander->begin();
    for (int i = 0; i < 8; i++) {
        expander->pinMode(i, OUTPUT);
    }
    for (int i = 0; i < 8; i++) {
        setPin(i, i == MUX_BRAKE_BLINK_PIN);
    }
}

void ExpanderUnit::setPin(const int pin, const bool enabled) const {
    expander->digitalWrite(pin, enabled ? HIGH : LOW);
}

void ExpanderUnit::setDrsEnabled(bool enabled) const {
    setPin(MUX_DRS_PIN, enabled);
}

void ExpanderUnit::setVescMosfetEnabled(const bool enabled) const {
    setPin(MUX_VESC_MOSFET_PIN, enabled);
}

void ExpanderUnit::setMotorFanEnabled(const bool enabled) const {
    setPin(MUX_MOTOR_FAN_PIN, enabled);
}

void ExpanderUnit::setOverallFanEnabled(const bool enabled) const {
    setPin(MUX_OVERALL_FAN_PIN, enabled);
}

void ExpanderUnit::setBrakeLedEnabled(const bool enabled) const {
    setPin(MUX_BRAKE_BLINK_PIN, !enabled);
    setPin(MUX_BRAKE_CONSTANT_PIN, enabled);
}
