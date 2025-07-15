#pragma once

#include "Arduino.h"

namespace Display {
    void setup();
    void showTelemetry(const String &text);
    void showGyroscope(const String &text);
    void showBattery(const String &text);
} // namespace Display
