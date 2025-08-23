#pragma once

#include <Arduino.h>

#include "WString.h"

class Gyroscope {
public:
    void setup();
    String getAcceleration() const;
    void readIMU() const;

private:
    boolean connected = false;
};
