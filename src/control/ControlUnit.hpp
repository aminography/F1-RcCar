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
    void loop() const;

private:
    DriveMode driveMode = DriveModes::SPORT;

    RadioLink radioLink;
    VescUnit vescUnit;
    Servo steeringServo;
    Servo drsServo;
    MuxUnit muxUnit;
    Gyroscope gyroscope;

    void onRadioChannelsReceived(const float *values);
    void updateDriveMode(float value);
    void updateThrottleState(float throttleValue, float brakeValue) const;
    void updateSteeringState(float value);
    void updateDrsState(float value);
};
