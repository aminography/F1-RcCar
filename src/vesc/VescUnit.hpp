#pragma once

#include "VescUart.h"
#include "VescSerialInterface.hpp"

class VescUnit {
public:
    struct TelemetryData {
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
        int id;
        int errorCode;
        String error;
        int fwVersionMajor;
        int fwVersionMinor;
    };

    void setup();
    void setDuty(float value) const;
    void setBrakeCurrent(float value) const;
    TelemetryData fetchTelemetryData() const;

private:
    VescSerialInterface *vesc = nullptr;
    static String getFaultCodeName(mc_fault_code code);
};
