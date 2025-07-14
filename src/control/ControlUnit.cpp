#include "control/ControlUnit.hpp"

#include <ESP32Servo.h>

#include "display/Display.hpp"
#include "task/TaskScheduler.hpp"

void ControlUnit::setup() {
    radioLink.setup();
    radioLink.setChannelCallback([this](const float *values) { onRadioChannelsReceived(values); });
    Scheduler.every(100, [this] {
        const VescUnit::TelemetryData telemetry = vescUnit.fetchTelemetryData();
        if (telemetry.error == FAULT_CODE_NONE) {
            Display::showTelemetry("RPM: " + String(static_cast<int>(telemetry.rpm)));
        } else {
            Display::showTelemetry("Error: " + String(telemetry.error));
        }
        Display::showGyroscope(gyroscope.getAcceleration());
        // radioLink.sendTelemetryD ata();
    });

    // Scheduler.every(200, [this] { radioLink.printAllChannels(); });

    vescUnit.setup();
    muxUnit.setup();
    gyroscope.setup();

    steeringServo.setPeriodHertz(50);
    steeringServo.attach(STEERING_SERVO_SIGNAL_PIN, 500, 2500);

    drsServo.setPeriodHertz(50);
    drsServo.attach(DRS_SERVO_SIGNAL_PIN, 500, 2500);
}

void ControlUnit::loop() { radioLink.update(); }

void ControlUnit::onRadioChannelsReceived(const float *values) {
    updateDriveMode(values[CH_9_DRIVE_MODE]);
    vescUnit.setDuty(scaleThrottle(driveMode, values[CH_2_THROTTLE], values[CH_5_BRAKE]));

    steeringServo.write(mapSteeringValue(driveMode, values[CH_1_STEERING_SERVO]));

    updateDRS(values[CH_7_DRS_ENABLED]);

    muxUnit.setBrakeLedEnabled(values[CH_5_BRAKE] > 0.5);
    muxUnit.setVescMosfetEnabled(values[CH_6_VESC_MOSFET] > 0.5);
    muxUnit.setMotorFanEnabled(values[CH_8_MOTOR_FAN] > 0.5);
}

float ControlUnit::mapSteeringValue(const DriveMode &driveMode, const float value) {
    // mapping in range [60, 106]
    return (value * 1000 + 999) * (106 - 60) / (999 + 999) + 60;
}

void ControlUnit::updateDRS(const float value) {
    static int previous = 0;
    const int current = value < 0.5 ? 0 : 180;
    if (current != previous) {
        previous = current;
        muxUnit.setDrsEnabled(true);
        Scheduler.once(100, [this, current] { drsServo.write(current); });
        Scheduler.once(1000, [this] { muxUnit.setDrsEnabled(false); });
    }
}

void ControlUnit::updateDriveMode(const float value) {
    if (value > 0.5) driveMode = DriveModes::SPORT;
    else if (value > -0.5) driveMode = DriveModes::ECO;
    else driveMode = DriveModes::DRAG;
}

float ControlUnit::scaleThrottle(const DriveMode &driveMode, const float throttleValue, const float brakeValue) {
    if (fabs(throttleValue) < 0.05) return 0;
    return throttleValue * driveMode.maxCurrent;

    // static float previousCurrent = 0.0;
    // static bool reversing = false;
    // static unsigned long brakeTimeStart = 0;
    // static constexpr unsigned long reverseHoldTime = 1000; // 1 second to activate reverse
    //
    // const bool inDeadZone = fabs(throttleInput) < 0.05;
    // const bool applyingBrake = brakeInput > 0.5f;
    //
    // float targetCurrent = 0.0;
    //
    // if (inDeadZone || applyingBrake) {
    //     // Start brake timer if not reversing yet
    //     if (!reversing) {
    //         if (brakeTimeStart == 0) {
    //             brakeTimeStart = millis();
    //         } else if (millis() - brakeTimeStart > reverseHoldTime) {
    //             reversing = true;
    //         }
    //     }
    //
    //     // Apply braking current
    //     targetCurrent = driveMode.brakeCurrent;
    // } else {
    //     // Reset brake timer when throttle is applied
    //     brakeTimeStart = 0;
    //
    //     if (reversing && throttleInput > 0.2f) {
    //         reversing = false; // switch back to forward when strong input
    //     }
    //
    //     // Calculate target drive current
    //     const float current = throttleInput * driveMode.maxCurrent;
    //
    //     if (reversing) {
    //         targetCurrent = -fabs(current); // force reverse direction
    //     } else {
    //         targetCurrent = fabs(current); // forward only
    //     }
    // }
    //
    // // Apply acceleration ramping (slew rate limiter)
    // float delta = targetCurrent - previousCurrent;
    // const float maxDelta = driveMode.accelerationRamp;
    //
    // if (delta > maxDelta) {
    //     delta = maxDelta;
    // } else if (delta < -maxDelta) {
    //     delta = -maxDelta;
    // }
    //
    // previousCurrent += delta;
    // return previousCurrent;
}
