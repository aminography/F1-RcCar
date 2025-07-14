#include "control/ControlUnit.hpp"

#include <ESP32Servo.h>

#include "control/DriveMode.hpp"
#include "display/Display.hpp"
#include "gyro/Gyroscope.hpp"
#include "mux/MuxUnit.hpp"
#include "radio/RadioLink.hpp"
#include "task/TaskScheduler.hpp"
#include "vesc/VescUnit.hpp"

RadioLink radioLink;
VescUnit vescUnit;
Servo steeringServo;
Servo drsServo;
MuxUnit muxUnit;
Gyroscope gyroscope;

DriveMode driveMode = DriveModes::SPORT;

void ControlUnit::setup() {
    radioLink.setup();
    radioLink.setChannelCallback([this](float* values) { onRadioChannelsReceived(values); });
    Scheduler.every(100, []() {
        VescUnit::TelemeteryData telemetery = vescUnit.fetchTelemeteryData();
        Display::showTelemetery("RPM: " + String((int)telemetery.rpm));
        Display::showGyroscope(gyroscope.getAcceleration());
        // radioLink.sendTelemeteryD ata();
    });

    Scheduler.every(200, []() { radioLink.printAllChannels(); });

    vescUnit.setup();
    muxUnit.setup();
    gyroscope.setup();

    steeringServo.setPeriodHertz(50);
    steeringServo.attach(STEERING_SERVO_SIGNAL_PIN, 500, 2500);

    drsServo.setPeriodHertz(50);
    drsServo.attach(DRS_SERVO_SIGNAL_PIN, 500, 2500);
}

void ControlUnit::loop() { radioLink.update(); }

void ControlUnit::onRadioChannelsReceived(float* values) {
    updateDriveMode(values[CH_9_DRIVE_MODE]);
    vescUnit.setCurrent(scaleThrottle(values[CH_2_THROTTLE], values[CH_5_BRAKE]));

    steeringServo.write(mapSteeringValue(values[CH_1_STEERING_SERVO]));
    drsServo.write(values[CH_7_DRS_ENABLED] < 0.5 ? 0 : 180);

    muxUnit.setBrakeLedEnabled(values[CH_5_BRAKE] > 0.5);
    muxUnit.setVescMosfetEnabled(values[CH_6_VESC_MOSFET] > 0.5);
    muxUnit.setMotorFanEnabled(values[CH_8_MOTOR_FAN] > 0.5);
}

float ControlUnit::mapSteeringValue(float value) {
    // mapping in range [60, 106]
    return ((value * 1000 + 999) * (106 - 60)) / (999 + 999) + 60;
}

void ControlUnit::updateDriveMode(float value) {
    if (value < -0.5)
        driveMode = DriveModes::ECO;
    else if (value < 0.5)
        driveMode = DriveModes::SPORT;
    else
        driveMode = DriveModes::DRAG;
}

float ControlUnit::scaleThrottle(float throttleInput, float brakeInput) {
    static float previousCurrent = 0.0;
    static bool reversing = false;
    static unsigned long brakeTimeStart = 0;
    static const unsigned long reverseHoldTime = 1000;  // 1 second to activate reverse

    const bool inDeadzone = fabs(throttleInput) < 0.05;
    const bool applyingBrake = brakeInput > 0.5f;

    float targetCurrent = 0.0;

    if (inDeadzone || applyingBrake) {
        // Start brake timer if not reversing yet
        if (!reversing) {
            if (brakeTimeStart == 0) {
                brakeTimeStart = millis();
            } else if (millis() - brakeTimeStart > reverseHoldTime) {
                reversing = true;
            }
        }

        // Apply braking current
        targetCurrent = driveMode.brakeCurrent;

    } else {
        // Reset brake timer when throttle is applied
        brakeTimeStart = 0;

        if (reversing && throttleInput > 0.2f) {
            reversing = false;  // switch back to forward when strong input
        }

        // Calculate target drive current
        float current = throttleInput * driveMode.maxCurrent;

        if (reversing) {
            targetCurrent = -fabs(current);  // force reverse direction
        } else {
            targetCurrent = fabs(current);  // forward only
        }
    }

    // Apply acceleration ramping (slew rate limiter)
    float delta = targetCurrent - previousCurrent;
    float maxDelta = driveMode.accelerationRamp;

    if (delta > maxDelta) {
        delta = maxDelta;
    } else if (delta < -maxDelta) {
        delta = -maxDelta;
    }

    previousCurrent += delta;
    return previousCurrent;
}
