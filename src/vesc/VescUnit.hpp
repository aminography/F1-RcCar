#pragma once

#include "VescUart.h"
#include "vesc/VescSerialInterface.hpp"

class VescUnit {
    
   public:
    struct TelemeteryData {
        float avgMotorCurrent;
        float avgInputCurrent;
        float dutyCycleNow;
        float rpm;
        float inpVoltage;
        float ampHours;
        float ampHoursCharged;
        float wattHours;
        float wattHoursCharged;
        long tachometer;
        long tachometerAbs;
        float tempMosfet;
        float tempMotor;
        float pidPos;
        uint8_t id;
        mc_fault_code error;
        uint8_t fwVersionMajor;
        uint8_t fwVersionMinor;
    };

    void setup();
    void setCurrent(float value);
    TelemeteryData fetchTelemeteryData();

   private:
    VescSerialInterface* vesc = nullptr;
};
