#include "VescUnit.hpp"
#include "VescSerialInterface.hpp"

void VescUnit::setup() {
    vesc = new VescSerialInterface();
}

void VescUnit::setDuty(const float value) const {
    vesc->setDuty(value);
}

void VescUnit::setBrakeCurrent(const float value) const {
    vesc->setBrakeCurrent(value);
}

VescUnit::TelemetryData VescUnit::fetchTelemetryData() const {
    if (vesc->getVescValues()) {
        return TelemetryData{
            vesc->data.avgMotorCurrent,
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
            getFaultCodeName(vesc->data.error),
            vesc->fw_version.major,
            vesc->fw_version.minor
        };
    }
    return TelemetryData{};
}

String VescUnit::getFaultCodeName(const mc_fault_code code) {
    switch (code) {
        case FAULT_CODE_OVER_VOLTAGE: return "OVER_VOLTAGE";
        case FAULT_CODE_UNDER_VOLTAGE: return "UNDER_VOLTAGE";
        case FAULT_CODE_DRV: return "DRV";
        case FAULT_CODE_ABS_OVER_CURRENT: return "ABS_OVER_CURRENT";
        case FAULT_CODE_OVER_TEMP_FET: return "OVER_TEMP_FET";
        case FAULT_CODE_OVER_TEMP_MOTOR: return "OVER_TEMP_MOTOR";
        case FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE: return "GATE_DRIVER_OVER_VOLTAGE";
        case FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE: return "GATE_DRIVER_UNDER_VOLTAGE";
        case FAULT_CODE_MCU_UNDER_VOLTAGE: return "MCU_UNDER_VOLTAGE";
        case FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET: return "BOOTING_FROM_WATCHDOG_RESET";
        case FAULT_CODE_ENCODER_SPI: return "ENCODER_SPI";
        case FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE: return "ENCODER_SINCOS_BELOW_MIN_AMPLITUDE";
        case FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE: return "ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE";
        case FAULT_CODE_FLASH_CORRUPTION: return "FLASH_CORRUPTION";
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1: return "HIGH_OFFSET_CURRENT_SENSOR_1";
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2: return "HIGH_OFFSET_CURRENT_SENSOR_2";
        case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3: return "HIGH_OFFSET_CURRENT_SENSOR_3";
        case FAULT_CODE_UNBALANCED_CURRENTS: return "UNBALANCED_CURRENTS";
        case FAULT_CODE_BRK: return "BRK";
        case FAULT_CODE_RESOLVER_LOT: return "RESOLVER_LOT";
        case FAULT_CODE_RESOLVER_DOS: return "RESOLVER_DOS";
        case FAULT_CODE_RESOLVER_LOS: return "RESOLVER_LOS";
        case FAULT_CODE_FLASH_CORRUPTION_APP_CFG: return "FLASH_CORRUPTION_APP_CFG";
        case FAULT_CODE_FLASH_CORRUPTION_MC_CFG: return "FLASH_CORRUPTION_MC_CFG";
        case FAULT_CODE_ENCODER_NO_MAGNET: return "ENCODER_NO_MAGNET";
        case FAULT_CODE_ENCODER_MAGNET_TOO_STRONG: return "ENCODER_MAGNET_TOO_STRONG";
        case FAULT_CODE_PHASE_FILTER: return "PHASE_FILTER";
        default: return "UNKNOWN";
    }
}
