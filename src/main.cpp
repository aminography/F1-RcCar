#include "control/ControlUnit.hpp"
#include "display/Display.hpp"
#include "i2c/I2CBus.hpp"
#include "task/TaskScheduler.hpp"

ControlUnit controlUnit;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    I2CBus::setup();
    Display::setup();
    controlUnit.setup();
}

void loop() {
    Scheduler.loop();
    controlUnit.loop();
}
