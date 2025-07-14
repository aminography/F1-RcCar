#pragma once
#include <Arduino.h>

namespace Display {

void setup();
void showTelemetery(const String& text);
void showGyroscope(const String& text);

}  // namespace Display
