#pragma once
#include <Arduino.h>

class Gyroscope {
   public:
    void setup();
    String getAcceleration();
    void readIMU();
};
