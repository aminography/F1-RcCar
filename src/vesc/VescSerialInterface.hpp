#pragma once

#include <HardwareSerial.h>

#include "Address.hpp"
#include "VescUart.h"

class VescSerialInterface : public VescUart {
   public:
    VescSerialInterface() : VescUart() {
        VESC_SERIAL_PORT.begin(VESC_BAUD_RATE, SERIAL_8N1, VESC_RX_PIN, VESC_TX_PIN);
        setSerialPort(&VESC_SERIAL_PORT);
    }
};
