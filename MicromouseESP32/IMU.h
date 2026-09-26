#pragma once
#include <Arduino.h>
#include <Wire.h>

// Reads the MPU-6050 gyro directly (same method as Debug 06) - no library needed,
// and it works with the clone chips that the Adafruit library rejects.
class IMU {
  uint8_t addr = 0;
  float biasDps = 0;
  bool present(uint8_t a);
  void writeReg(uint8_t reg, uint8_t val);
  float readDps();
public:
  bool begin();      // keep the robot STILL while this runs (~1 s)
  float yawRate();   // radians/sec
};
