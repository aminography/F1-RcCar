#pragma once

struct DriveMode {
    const char *name;
    float maxPower;
    float brakeForce;
    float accelerationRamp;

    constexpr DriveMode(
        const char *name,
        const float maxPower,
        const float brakeForce,
        const float accelerationRamp
    ) : name(name),
        maxPower(maxPower),
        brakeForce(brakeForce),
        accelerationRamp(accelerationRamp) {
    }
};

namespace DriveModes {
    constexpr DriveMode ECO{
        "Eco",
        0.75f,
        -10.0f,
        0.01f
    };
    constexpr DriveMode SPORT{
        "Sport",
        1.0f,
        -20.0f,
        0.1f
    };
    constexpr DriveMode DRAG{
        "Drag",
        1.0f,
        -30.0f,
        0.2f
    };
} // namespace DriveModes
