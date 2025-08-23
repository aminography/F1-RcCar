#pragma once

#include "PCF8575.h"

class ExpanderUnit {
public:
    void setup();
    void setDefaults() const;
    // void setDrsPowerEnabled(bool enabled) const;
    void setVescMosfetEnabled(bool enabled) const;
    void setMotorFanEnabled(bool enabled) const;
    void setRearLedEnabled(bool enabled) const;
    void setRearStripeEnabled(bool enabled) const;

private:
    PCF8575 *expander = nullptr;
    void setPin(int pin, bool enabled) const;
};
