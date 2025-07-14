#include "display/Display.hpp"

#include <Wire.h>

#include "HT_SSD1306Wire.h"
#include "task/TaskScheduler.hpp"

SSD1306Wire* display = nullptr;
String telemetryText;
String gyroscopeText;

void refresh();
void VextON();
void VextOFF();
void drawFontFaceDemo();

void Display::setup() {
    // addr , freq , i2c group , resolution , rst
    display = new SSD1306Wire(0x3C, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

    Serial.println();
    Serial.println();
    VextON();
    display->init();
    Scheduler.every(50, []() { refresh(); });
}

void Display::showTelemetry(const String& text) { telemetryText = text; }

void Display::showGyroscope(const String& text) { gyroscopeText = text; }

void refresh() {
    display->clear();
    drawFontFaceDemo();
    display->display();
}

void drawFontFaceDemo() {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);

    unsigned long time = millis();
    char timeStr[9];
    sprintf(timeStr, "%02u:%02u:%02u", time / 60000, (time / 1000) % 60, (time / 10) % 100);
    display->drawString(0, 0, "UpTime: " + String(timeStr));

    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 18, telemetryText);

    display->setFont(ArialMT_Plain_10);
    display->drawString(0, 38, gyroscopeText);
}

void VextON() {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
}

void VextOFF() {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, HIGH);
}
