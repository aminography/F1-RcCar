#pragma once

#include "radio/CrsfSerialInterface.hpp"

class RadioLink {
public:
    typedef std::function<void(float *)> ChannelCallback;

    void setup();
    void update() const;
    void sendTelemetryData();
    void printAllChannels() const;
    static void setChannelCallback(const ChannelCallback &);

private:
#define CHANNEL_COUNT crsfProtocol::RC_CHANNEL_COUNT

    CrsfSerialInterface *crsf = nullptr;
    static float scaleValue(float microseconds);
};
