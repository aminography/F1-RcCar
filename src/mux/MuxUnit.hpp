#pragma once

#include "PCF8575.h"

class MuxUnit {

   public:
    void setup();
    void setVescMosfetEnabled(bool enabled);
    void setMotorFanEnabled(bool enabled);
    void setOverallFanEnabled(bool enabled);
    void setBrakeLedEnabled(bool enabled);

   private:
    PCF8575* expander = nullptr;
    void setPin(int pin, bool enabled);
};
