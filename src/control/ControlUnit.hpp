#pragma once

#include "DriveMode.hpp"
#include "ESP32Servo.h"
#include "gyro/Gyroscope.hpp"
#include "expander/ExpanderUnit.hpp"
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
    ExpanderUnit expanderUnit;
    Gyroscope gyroscope;

    void onRadioChannelsReceived(bool connected, const float *values);
    void startTelemetryHandling() const;
    void updateDriveMode(float value);
    void updateThrottleState(float throttleValue, bool isBraking) const;
    void updateSteeringState(float value);
    void updateDrsState(float value);
};
