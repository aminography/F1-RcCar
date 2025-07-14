#include "vesc/VescUnit.hpp"

#include <cmath>

#include "task/TaskScheduler.hpp"
#include "vesc/VescSerialInterface.hpp"

void VescUnit::setup() { vesc = new VescSerialInterface(); }

void VescUnit::setCurrent(float value) { vesc->setCurrent(value); }

VescUnit::TelemetryData VescUnit::fetchTelemetryData() {
    if (vesc->getVescValues()) {
        return VescUnit::TelemetryData{vesc->data.avgMotorCurrent,
                                        vesc->data.avgInputCurrent,
                                        vesc->data.dutyCycleNow,
                                        vesc->data.rpm,
                                        vesc->data.inpVoltage,
                                        vesc->data.ampHours,
                                        vesc->data.ampHoursCharged,
                                        vesc->data.wattHours,
                                        vesc->data.wattHoursCharged,
                                        vesc->data.tachometer,
                                        vesc->data.tachometerAbs,
                                        vesc->data.tempMosfet,
                                        vesc->data.tempMotor,
                                        vesc->data.pidPos,
                                        vesc->data.id,
                                        vesc->data.error,
                                        vesc->fw_version.major,
                                        vesc->fw_version.minor};
    }
    return VescUnit::TelemetryData{};
}
