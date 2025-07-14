#pragma once
#include "DriveMode.hpp"
#include "ESP32Servo.h"
#include "gyro/Gyroscope.hpp"
#include "mux/MuxUnit.hpp"
#include "radio/RadioLink.hpp"
#include "vesc/VescUnit.hpp"

class ControlUnit {
   public:
    void setup();
    void loop();

   private:
    DriveMode driveMode = DriveModes::SPORT;

    RadioLink radioLink;
    VescUnit vescUnit;
    Servo steeringServo;
    Servo drsServo;
    MuxUnit muxUnit;
    Gyroscope gyroscope;

    void onRadioChannelsReceived(const float* values);
    void updateDriveMode(float value);
    void updateDRS(float value);

    static float mapSteeringValue(const DriveMode &driveMode, float value);
    static float scaleThrottle(const DriveMode &driveMode, float throttleValue, float brakeValue);
};
