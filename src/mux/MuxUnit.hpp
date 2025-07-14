#pragma once

#include "PCF8575.h"

class MuxUnit {

   public:
    void setup();
    void setDrsEnabled(bool enabled) const;
    void setVescMosfetEnabled(bool enabled) const;
    void setMotorFanEnabled(bool enabled) const;
    void setOverallFanEnabled(bool enabled) const;
    void setBrakeLedEnabled(bool enabled) const;

   private:
    PCF8575* expander = nullptr;
    void setPin(int pin, bool enabled) const;
};
