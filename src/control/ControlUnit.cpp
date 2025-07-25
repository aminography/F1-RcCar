#include "ControlUnit.hpp"

#include "ESP32Servo.h"
#include "display/Display.hpp"
#include "task/TaskScheduler.hpp"

void ControlUnit::setup() {
    radioLink.setup();
    radioLink.setChannelCallback([this](const bool connected, const float *values) {
        onRadioChannelsReceived(connected, values);
    });

    vescUnit.setup();
    expanderUnit.setup();
    gyroscope.setup();

    steeringServo.setPeriodHertz(50);
    steeringServo.attach(STEERING_SERVO_SIGNAL_PIN, 500, 2500);

    drsServo.setPeriodHertz(50);
    drsServo.attach(DRS_SERVO_SIGNAL_PIN, 500, 2500);

    startTelemetryHandling();
    // Scheduler.every(200, [this] { radioLink.printAllChannels(); });
}

void ControlUnit::loop() const { radioLink.update(); }

void ControlUnit::onRadioChannelsReceived(const bool connected, const float *values) {
    if (connected) {
        updateDriveMode(values[CH_9_DRIVE_MODE]);
        updateSteeringState(values[CH_1_STEERING_SERVO]);
        updateDrsState(values[CH_7_DRS_ENABLED]);

        const bool isVescMosfetEnabled = values[CH_6_VESC_MOSFET] > 0.5;
        expanderUnit.setVescMosfetEnabled(isVescMosfetEnabled);
        if (isVescMosfetEnabled) {
            updateThrottleState(values[CH_2_THROTTLE], values[CH_5_BRAKE] > 0.5f);
        }

        expanderUnit.setBrakeLedEnabled(values[CH_5_BRAKE] > 0.5);
        expanderUnit.setMotorFanEnabled(values[CH_8_MOTOR_FAN] > 0.5);
    } else {
        expanderUnit.setDefaults();
    }
}

void ControlUnit::startTelemetryHandling() const {
    Scheduler.every(100, [this] {
        const VescUnit::TelemetryData telemetry = vescUnit.fetchTelemetryData();

        if (telemetry.errorCode == FAULT_CODE_NONE) {
            Display::showTelemetry("RPM: " + String(static_cast<int>(telemetry.rpm)));
        } else {
            Display::showTelemetry("Error: " + String(telemetry.error));
        }
        Display::showGyroscope(gyroscope.getAcceleration());
        // radioLink.sendTelemetryD ata();

        double battery_percentage = (telemetry.inpVoltage - 9.9) / (13.05 - 9.9) * 100;
        battery_percentage = min(max(battery_percentage, 0.0), 100.0);
        Display::showBattery(
            String(telemetry.inpVoltage) + "v   " + String(static_cast<int>(battery_percentage)) + "%");
    });
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

    static const TaskScheduler::Callback drsPowerOff = [this] {
        debounceId = 0;
        expanderUnit.setDrsPowerEnabled(false);
    };

    const int current = value < 0.5 ? 0 : 180;
    if (current != previous) {
        previous = current;
        expanderUnit.setDrsPowerEnabled(true);
        drsServo.write(current);

        static constexpr int DEBOUNCE_DELAY = 1500;
        if (debounceId == 0) {
            debounceId = Scheduler.once(DEBOUNCE_DELAY, drsPowerOff);
        } else {
            Scheduler.reschedule(debounceId, DEBOUNCE_DELAY);
        }
    }
}

void ControlUnit::updateDriveMode(const float value) {
    if (value > 0.5) driveMode = DriveModes::SPORT;
    else if (value > -0.5) driveMode = DriveModes::ECO;
    else driveMode = DriveModes::DRAG;
}

void ControlUnit::updateThrottleState(const float throttleValue, const bool isBraking) const {
    // Throttle shaping helps to feel more like analog gas pedal behavior, especially useful at low speeds.
    // y = sign(x) * (∣x∣^expo)
    const float sign = throttleValue < 0 ? -1 : 1;
    const float shapedThrottle = sign * pow(fabs(throttleValue), driveMode.throttleExpo);

    static float currentThrottle = 0.0;
    float targetThrottle = 0;

    if (isBraking) {
        currentThrottle = 0;
        vescUnit.setDuty(currentThrottle);
        vescUnit.setBrakeCurrent(driveMode.brakeForce);
        return;
    }

    if (fabs(shapedThrottle) < 0.05) {
        targetThrottle = 0; // filtering out dead zone
    } else if (shapedThrottle > 0) {
        targetThrottle = shapedThrottle * driveMode.forwardPower;
    } else {
        targetThrottle = shapedThrottle * driveMode.reversePower;
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
    currentThrottle = min(max(currentThrottle, -1.0f), 1.0f);

    vescUnit.setDuty(currentThrottle);
}
