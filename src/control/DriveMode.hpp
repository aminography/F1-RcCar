#pragma once

struct DriveMode {
    const char *name;
    float maxPower;
    float reversePower;
    float brakeForce;
    float accelerationRamp;

    constexpr DriveMode(
        const char *name,
        const float maxPower,
        const float reversePower,
        const float brakeForce,
        const float accelerationRamp
    ) : name(name),
        maxPower(maxPower),
        reversePower(reversePower),
        brakeForce(brakeForce),
        accelerationRamp(accelerationRamp) {
    }
};

namespace DriveModes {
    constexpr DriveMode ECO{
        "Eco",
        0.75f,
        0.20f,
        -10.0f,
        0.01f
    };
    constexpr DriveMode SPORT{
        "Sport",
        1.0f,
        0.34f,
        -20.0f,
        0.1f
    };
    constexpr DriveMode DRAG{
        "Drag",
        1.0f,
        0.34f,
        -30.0f,
        0.2f
    };
} // namespace DriveModes
