#include "ControlUnit.hpp"

#include "ESP32Servo.h"
#include "display/Display.hpp"
#include "task/TaskScheduler.hpp"

void ControlUnit::setup() {
    radioLink.setup();
    radioLink.setChannelCallback([this](const float *values) { onRadioChannelsReceived(values); });
    Scheduler.every(100, [this] {
        const VescUnit::TelemetryData telemetry = vescUnit.fetchTelemetryData();
        if (telemetry.errorCode == FAULT_CODE_NONE) {
            Display::showTelemetry("RPM: " + String(static_cast<int>(telemetry.rpm)));
        } else {
            Display::showTelemetry("Error: " + String(telemetry.error));
        }
        Display::showGyroscope(gyroscope.getAcceleration());
        // radioLink.sendTelemetryD ata();
    });

    // Scheduler.every(200, [this] { radioLink.printAllChannels(); });

    vescUnit.setup();
    expanderUnit.setup();
    gyroscope.setup();

    steeringServo.setPeriodHertz(50);
    steeringServo.attach(STEERING_SERVO_SIGNAL_PIN, 500, 2500);

    drsServo.setPeriodHertz(50);
    drsServo.attach(DRS_SERVO_SIGNAL_PIN, 500, 2500);
}

void ControlUnit::loop() const { radioLink.update(); }

void ControlUnit::onRadioChannelsReceived(const float *values) {
    updateDriveMode(values[CH_9_DRIVE_MODE]);
    updateThrottleState(values[CH_2_THROTTLE], values[CH_5_BRAKE]);
    updateSteeringState(values[CH_1_STEERING_SERVO]);
    updateDrsState(values[CH_7_DRS_ENABLED]);

    expanderUnit.setBrakeLedEnabled(values[CH_5_BRAKE] > 0.5);
    expanderUnit.setVescMosfetEnabled(values[CH_6_VESC_MOSFET] > 0.5);
    expanderUnit.setMotorFanEnabled(values[CH_8_MOTOR_FAN] > 0.5);
}

void ControlUnit::updateSteeringState(const float value) {
    // TODO: Consider driveMode in steering responsiveness
    // mapping [-1, 1] to range of [60, 106]
    constexpr int min = 60;
    constexpr int max = 106;
    constexpr int precision = 1000;
    const int scaledValue = static_cast<int>(value * precision);
    const int angle = (scaledValue + precision - 1) * (max - min) / (2 * (precision - 1)) + min;
    steeringServo.write(angle);
}

void ControlUnit::updateDrsState(const float value) {
    static int previous = 0;
    static int debounceId = 0;
    static const TaskScheduler::Callback disableDrs = [this] { expanderUnit.setDrsEnabled(false); };

    const int current = value < 0.5 ? 0 : 180;
    if (current != previous) {
        previous = current;
        expanderUnit.setDrsEnabled(true);
        drsServo.write(current);

        if (debounceId == 0) {
            debounceId = Scheduler.once(1000, disableDrs);
        } else {
            Scheduler.reschedule(debounceId, 1000);
        }
    }
}

void ControlUnit::updateDriveMode(const float value) {
    if (value > 0.5) driveMode = DriveModes::SPORT;
    else if (value > -0.5) driveMode = DriveModes::ECO;
    else driveMode = DriveModes::DRAG;
}

void ControlUnit::updateThrottleState(const float throttleValue, const float brakeValue) const {
    static float currentThrottle = 0.0;
    float targetThrottle = throttleValue * driveMode.maxPower;
    if (fabs(targetThrottle) < 0.05) targetThrottle = 0; // filtering out dead zone

    if (brakeValue > 0.5f) {
        vescUnit.setBrakeCurrent(driveMode.brakeForce);
        targetThrottle = 0;
    } else {
        vescUnit.setBrakeCurrent(0);
    }

    // Apply acceleration ramping
    float delta = targetThrottle - currentThrottle;
    const float maxDelta = driveMode.accelerationRamp;

    if (delta > maxDelta) {
        delta = maxDelta;
    } else if (delta < -maxDelta) {
        delta = -maxDelta;
    }

    currentThrottle += delta;
    vescUnit.setDuty(currentThrottle);
}
