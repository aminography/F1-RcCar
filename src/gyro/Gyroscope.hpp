#pragma once

#include "WString.h"

class Gyroscope {
public:
    static void setup();
    static String getAcceleration();
    static void readIMU();
};
