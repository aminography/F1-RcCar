#include "ExpanderUnit.hpp"

#include "Address.hpp"
#include "PCF8575.h"

void ExpanderUnit::setup() {
    expander = new PCF8575(&I2C_WIRE, I2C_EXPANDER_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN);
    expander->begin();
    for (int i = 0; i < 8; i++) {
        expander->pinMode(i, OUTPUT);
    }
    setDefaults();
}

void ExpanderUnit::setPin(const int pin, const bool enabled) const {
    expander->digitalWrite(pin, enabled ? HIGH : LOW);
}

void ExpanderUnit::setDefaults() const {
    for (int i = 0; i < 8; i++) {
        setPin(i, false);
    }
}

// void ExpanderUnit::setDrsPowerEnabled(bool enabled) const {
//     setPin(EXPANDER_DRS_POWER_PIN, enabled);
// }

void ExpanderUnit::setVescMosfetEnabled(const bool enabled) const {
    setPin(EXPANDER_VESC_MOSFET_PIN, enabled);
}

void ExpanderUnit::setMotorFanEnabled(const bool enabled) const {
    setPin(EXPANDER_MOTOR_FAN_PIN, enabled);
}

void ExpanderUnit::setRearLedEnabled(const bool enabled) const {
    setPin(EXPANDER_REAR_LED_PIN, enabled);
}

void ExpanderUnit::setRearStripeEnabled(const bool enabled) const {
    setPin(EXPANDER_REAR_STRIPE_PIN, enabled);
}
