#include "radio/RadioLink.hpp"

#include "Arduino.h"
#include "SerialReceiver/CRSF/CRSF.hpp"
#include "radio/CrsfSerialInterface.hpp"

static RadioLink::channelCallback _channelCallback;

void RadioLink::setup() {
    crsf = new CrsfSerialInterface();

    if (!crsf->begin()) {
        Serial.println("CRSF initialization failed!");
    }

    crsf->setRcChannelsCallback([](serialReceiverLayer::rcChannels_t *rcChannels) {
        if (!rcChannels->failsafe) {
            // values are in range [-1.0, 1.0]
            float values[CHANNEL_COUNT];
            std::transform(rcChannels->value, rcChannels->value + CHANNEL_COUNT, values,
                           [](const uint16_t item) { return scaleValue(item); });

            if (_channelCallback != nullptr) {
                _channelCallback(values);
            }
        } else {
            Serial.println("RadioLink failsafe active!");
        }
    });
}

void RadioLink::update() { crsf->update(); }

void RadioLink::setChannelCallback(channelCallback callback) { _channelCallback = callback; }

void RadioLink::sendTelemetryData() {
    // crsf->telessmetryWriteBattery(throttle, throttle, throttle, throttle);
}

void RadioLink::printAllChannels() {
    static unsigned long lastPrint = 0;
    unsigned long now = millis();
    if (now - lastPrint >= 100) {
        lastPrint = now;

        Serial.print("RC Channels <");
        for (int i = 1; i <= CHANNEL_COUNT; i++) {
            Serial.print(String(i));
            Serial.print(": ");
            Serial.print(scaleValue(crsf->getChannel(i)));
            if (i < CHANNEL_COUNT) Serial.print("   ");
        }
        Serial.println(">");
    }
}

int RadioLink::scaleValue(const int microseconds) {
    const int scale = 1000;
    float scaled_value = (microseconds - 992) * scale / 820;
    if (scaled_value < -scale) scaled_value = -scale;
    if (scaled_value > scale) scaled_value = scale;
    return scaled_value / scale;
}
