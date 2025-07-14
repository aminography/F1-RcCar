#pragma once

struct DriveMode {
    const char* name;
    float maxCurrent;
    float brakeCurrent;
    float accelerationRamp;

    constexpr DriveMode(const char* name, float maxCurrent, float brakeCurrent,
                        float accelerationRamp)
        : name(name),
          maxCurrent(maxCurrent),
          brakeCurrent(brakeCurrent),
          accelerationRamp(accelerationRamp) {}
};

namespace DriveModes {

constexpr DriveMode ECO{"Eco", 15.0f, -10.0f, 0.3f};
constexpr DriveMode SPORT{"Sport", 30.0f, -20.0f, 0.6f};
constexpr DriveMode DRAG{"Drag", 45.0f, -35.0f, 1.0f};

}  // namespace DriveModes
