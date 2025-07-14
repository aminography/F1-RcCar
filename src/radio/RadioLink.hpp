#pragma once

#include "radio/CrsfSerialInterface.hpp"

class RadioLink {
   public:
    typedef std::function<void(float*)> channelCallback;

    void setup();
    void update();
    void setChannelCallback(channelCallback);
    void sendTelemetryData();
    void printAllChannels();

   private:
    #define CHANNEL_COUNT crsfProtocol::RC_CHANNEL_COUNT

    CrsfSerialInterface* crsf = nullptr;
    static int scaleValue(int microseconds);
};
