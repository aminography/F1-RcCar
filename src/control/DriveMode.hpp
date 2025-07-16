#pragma once

struct DriveMode {
    const char *name;
    float maxPower;
    float reversePower;
    float brakeForce;
    float accelerationRamp;
    float throttleExpo;

    constexpr DriveMode(
        const char *name,
        const float maxPower,
        const float reversePower,
        const float brakeForce,
        const float accelerationRamp,
        const float throttleExpo
    ) : name(name),
        maxPower(maxPower),
        reversePower(reversePower),
        brakeForce(brakeForce),
        accelerationRamp(accelerationRamp),
        throttleExpo(throttleExpo) {
    }
};

namespace DriveModes {
    constexpr DriveMode ECO{
        "Eco",
        0.75f,
        0.25f,
        15.0f,
        0.05f,
        2.0f // Smooth parabolic shape, less sensitive at low throttle.
    };
    constexpr DriveMode SPORT{
        "Sport",
        1.0f,
        0.34f,
        25.0f,
        0.15f,
        1.5f // Responsive but not twitchy. Still smooth near center but ramps up harder.
    };
    constexpr DriveMode DRAG{
        "Drag",
        1.0f,
        0.34f,
        30.0f,
        0.25f,
        1.0f // Aggressive throttle response (full torque), even with slight trigger movement.
    };
} // namespace DriveModes
