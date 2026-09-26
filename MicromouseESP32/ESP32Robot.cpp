#include "ESP32Robot.h"
#include "Config.h"
#include "Motors.h"
#include "Encoders.h"
#include <Wire.h>
#include <math.h>

bool ESP32Robot::begin() {
  Motors::begin();
  Encoders::begin();
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  if (!sensors.begin()) { halt("Distance sensor init failed"); return false; }
  if (!imu.begin())     { halt("IMU init failed"); return false; }
  ready = true;
  return true;
}

void ESP32Robot::halt(const char* why) {
  Motors::stop();
  fault = true;
  Serial.print("HALT: ");
  Serial.println(why);
}

void ESP32Robot::stop() { Motors::stop(); }

void ESP32Robot::printStatus() {
  int l = sensors.left(), f = sensors.front(), r = sensors.right();
  auto show = [](const char* n, int mm) {
    if (mm < 0) Serial.printf("%s:ERR  ", n);
    else if (mm >= NO_WALL_MM) Serial.printf("%s:---  ", n);
    else Serial.printf("%s:%dmm(%s)  ", n, mm, mm < WALL_MM ? "WALL" : "open");
  };
  show("L", l); show("F", f); show("R", r);
  Serial.printf(" encL:%ld encR:%ld  gyro:%.1fdps\n",
                Encoders::left(), Encoders::right(), imu.yawRate() * 180.0f / PI);
}

bool ESP32Robot::readWall(int d) {
  if (fault) return true;
  int mm = d == 0 ? sensors.left() : d == 1 ? sensors.front() : sensors.right();
  if (mm < 0) { halt("Sensor stopped responding"); return true; }
  return mm < WALL_MM;
}

void ESP32Robot::moveForward(int n) {
  if (fault || n <= 0) return;
  // One cell at a time for now (slow but reliable).
  const long ticks = lroundf(CELL_MM / (PI * WHEEL_DIAMETER_MM) * COUNTS_PER_WHEEL_REV);
  for (int cell = 0; cell < n && !fault; cell++) {
    Encoders::reset();
    unsigned long start = millis();
    while (!fault) {
      long l = labs(Encoders::left()), r = labs(Encoders::right());
      if ((l + r) / 2 >= ticks) break;
      if (millis() - start > MOVE_TIMEOUT_MS) { halt("Forward timeout (check encoders)"); break; }
      int frontMM = sensors.front();
      if (frontMM < 0) { halt("Sensor stopped responding"); break; }
      if (frontMM < FRONT_STOP_MM) { halt("Obstacle too close"); break; }
      // Keep straight: slow down whichever wheel is ahead
      long error = l - r;
      int correction = constrain((int)(error / 4), -35, 35);
      Motors::drive(MOTOR_PWM - correction, MOTOR_PWM + correction);
      delay(5);
    }
    Motors::stop();
    delay(120);
  }
}

void ESP32Robot::turn(bool left) {
  if (fault) return;
  const float target = TURN_STOP_DEG * PI / 180.0f;
  unsigned long start = millis(), prev = micros();
  float angle = 0;
  while (fabsf(angle) < target) {
    if (millis() - start > TURN_TIMEOUT_MS) { halt("Turn timeout (check IMU)"); return; }
    unsigned long now = micros();
    float dt = (now - prev) / 1000000.0f;
    prev = now;
    angle += imu.yawRate() * dt;
    Motors::drive(left ? -MOTOR_PWM : MOTOR_PWM, left ? MOTOR_PWM : -MOTOR_PWM);
    delay(4);
  }
  Motors::stop();
  delay(150);
}
