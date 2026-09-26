#pragma once
#include <Arduino.h>

// ===================== PINS (match the event wiring) =====================
// Motor driver (DRI0044): left = DIR1/PWM1, right = DIR2/PWM2
constexpr int L_DIR = 0, L_PWM = 2;     // you wired DIR1 -> GPIO0, PWM1 -> GPIO2
constexpr int R_DIR = 3, R_PWM = 10;    // DIR2 -> GPIO3, PWM2 -> GPIO10

// Flip to true if that wheel spins BACKWARDS when you send 'f'
// (easier than swapping motor wires)
constexpr bool INVERT_LEFT  = false;
constexpr bool INVERT_RIGHT = false;

// I2C bus shared by the 3 distance sensors and the IMU
constexpr int SDA_PIN = 6, SCL_PIN = 7;

// Distance sensor XSHUT pins
constexpr int X_LEFT = 18, X_FRONT = 1, X_RIGHT = 20;

// Wheel encoders (C1 = A, C2 = B). Left matches the event guide.
// Right: wire C1 -> GPIO11, C2 -> GPIO23 (or change these to where you wired it)
constexpr int L_A = 21, L_B = 22;
constexpr int R_A = 11, R_B = 23;

// ===================== CALIBRATE THESE ON THE REAL MAZE =====================
constexpr float CELL_MM = 180.0f;
constexpr float WHEEL_DIAMETER_MM = 32.0f;       // measure your wheel with a ruler
constexpr float COUNTS_PER_WHEEL_REV = 600.0f;   // spin wheel 1 turn by hand, read with 's'
constexpr int   WALL_MM = 110;        // reading below this = wall is there
constexpr int   FRONT_STOP_MM = 45;   // emergency stop if something this close in front
constexpr int   MOTOR_PWM = 105;      // speed 0..255, start low
constexpr float TURN_STOP_DEG = 85.0f; // stops a bit early because the robot coasts; tune to land on 90

constexpr unsigned long MOVE_TIMEOUT_MS = 5000, TURN_TIMEOUT_MS = 4000;

// Sensor readings at/after this mean "nothing in range" (open corridor)
constexpr uint16_t MAX_VALID_MM = 2000;
constexpr int NO_WALL_MM = 9999;
