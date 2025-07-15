#pragma once

#include "HardwareSerial.h"
#include "Address.hpp"
#include "CRSFforArduino.hpp"

class CrsfSerialInterface : public CRSFforArduino {
public:
    CrsfSerialInterface() : CRSFforArduino(&RADIO_SERIAL_PORT, RADIO_RX_PIN, RADIO_TX_PIN) {}

    bool begin() { return CRSFforArduino::begin(RADIO_BAUD_RATE); }
};
