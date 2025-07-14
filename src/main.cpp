#include <Arduino.h>

#include "control/ControlUnit.hpp"
#include "display/Display.hpp"
#include "i2c/I2CBus.hpp"
#include "task/TaskScheduler.hpp"

ControlUnit controlUnit;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    I2CBus::setup();
    Display::setup();

    controlUnit.setup();
}

void loop() {
    Scheduler.loop();
    controlUnit.loop();
}

// // // int blinkId = Scheduler.every(500, []() { digitalWrite(2, !digitalRead(2)); });
// // // Scheduler.once(3000, []() {
// // //     Serial.println("⏸️ Pause");
// // //     Scheduler.pause(blinkId);
// // // });
// // // Scheduler.once(6000, []() {
// // //     Serial.println("▶️ Resume");
// // //     Scheduler.resume(blinkId);
// // // });
// // // Scheduler.once(9000, []() {
// // //     Serial.println("⚡ Faster");
// // //     Scheduler.reschedule(blinkId, 200);
// // // });
// // // Scheduler.once(12000, []() {
// // //     Serial.println("🛑 Cancel");
// // //     Scheduler.cancel(blinkId);
// // //     digitalWrite(2, LOW);
// // // });
