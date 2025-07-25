#include "RadioLink.hpp"

#include "Arduino.h"
#include "CrsfSerialInterface.hpp"

static RadioLink::ChannelCallback channelCallback;

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

            if (channelCallback != nullptr) {
                channelCallback(true, values);
            }
        } else {
            if (channelCallback != nullptr) {
                channelCallback(false, nullptr);
            }
        }
    });
}

void RadioLink::update() const {
    crsf->update();
}

void RadioLink::setChannelCallback(const ChannelCallback &callback) {
    channelCallback = callback;
}

void RadioLink::sendTelemetryData() {
    // crsf->telessmetryWriteBattery(throttle, throttle, throttle, throttle);
}

void RadioLink::printAllChannels() const {
    static unsigned long lastPrint = 0;
    const unsigned long now = millis();
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

float RadioLink::scaleValue(const float microseconds) {
    constexpr int scale = 1000;
    float scaled_value = (microseconds - 992.0f) * scale / 820.0f;
    if (scaled_value < -scale) scaled_value = -scale;
    if (scaled_value > scale) scaled_value = scale;
    return scaled_value / scale;
}
