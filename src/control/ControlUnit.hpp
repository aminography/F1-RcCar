#pragma once

#include "control/DriveMode.hpp"

class ControlUnit {
   public:
    void setup();
    void loop();

   private:
    void onRadioChannelsReceived(float* values);
    void updateDriveMode(float value);
    float mapSteeringValue(float value);
    float scaleThrottle(float throttleInput, float brakeInput);
};
