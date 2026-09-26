#pragma once
#include <Arduino.h>
// Check the actual ESP32-C6 board pinout: GPIO22/23 may not be exposed.
// L_DIR moved off GPIO0: GPIO0 is a boot-strapping pin on ESP32 parts and
// can force download mode if pulled low externally at reset. Re-check this
// against your board's actual strapping-pin list before relying on it.
constexpr int L_DIR=1,L_PWM=2,R_DIR=3,R_PWM=10;
// STBY must be driven HIGH for most TB6612-style drivers (e.g. DRI0044) to
// output anything. If your breakout ties STBY high on-board, this is
// harmless; verify with a multimeter either way. Pick a free, non-strapping
// GPIO — 4 is a placeholder, confirm it's exposed and unused on your board.
constexpr int STBY_PIN=4;
constexpr int SDA_PIN=6,SCL_PIN=7;
constexpr int X_LEFT=18,X_FRONT=19,X_RIGHT=20;
constexpr int L_A=21,L_B=22,R_A=11,R_B=23;
constexpr float CELL_MM=180.0f;
constexpr float WHEEL_DIAMETER_MM=32.0f; // MEASURE YOUR WHEELS
constexpr float COUNTS_PER_WHEEL_REV=600.0f; // CALIBRATE for gearbox and x2 decoding
constexpr int WALL_MM=110; // CALIBRATE at cell centre
constexpr int FRONT_STOP_MM=45; // CALIBRATE for your chassis
constexpr int MOTOR_PWM=105; // 0..255; start low
constexpr float TURN_RATE_RAD_S=1.5f;
constexpr unsigned long MOVE_TIMEOUT_MS=5000,TURN_TIMEOUT_MS=4000;
// Motor driver VM must be separately supplied at your motors' rated voltage.
