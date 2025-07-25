#pragma once

#include "Wire.h"

namespace Address {

#define RADIO_RX_PIN 5
#define RADIO_TX_PIN 6
#define RADIO_BAUD_RATE 420000
#define RADIO_SERIAL_PORT Serial1

#define CH_1_STEERING_SERVO 0
#define CH_2_THROTTLE 1
#define CH_5_BRAKE 4
#define CH_6_VESC_MOSFET 5
#define CH_7_DRS_ENABLED 6
#define CH_8_MOTOR_FAN 7
#define CH_9_DRIVE_MODE 8

#define VESC_RX_PIN 3
#define VESC_TX_PIN 4
#define VESC_BAUD_RATE 115200
#define VESC_SERIAL_PORT Serial2

#define I2C_SCL_PIN 1
#define I2C_SDA_PIN 2
#define I2C_WIRE Wire1
#define I2C_EXPANDER_ADDRESS 0x20
#define I2C_GYROSCOPE_ADDRESS 0x6A

#define STEERING_SERVO_SIGNAL_PIN 42
#define DRS_SERVO_SIGNAL_PIN 47

#define EXPANDER_DRS_POWER_PIN 0
#define EXPANDER_MOTOR_FAN_PIN 1
#define EXPANDER_OVERALL_FAN_PIN 2
#define EXPANDER_BRAKE_BLINK_PIN 3
#define EXPANDER_BRAKE_CONSTANT_PIN 4
#define EXPANDER_VESC_MOSFET_PIN 7

}; // namespace Address
