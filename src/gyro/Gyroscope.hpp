#pragma once
#include <Arduino.h>

class Gyroscope {
   public:
    static void setup();
    static String getAcceleration();
    static void readIMU();
};
