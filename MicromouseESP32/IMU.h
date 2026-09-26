#pragma once
#include <Adafruit_MPU6050.h>
class IMU {
 Adafruit_MPU6050 sensor;
 float bias=0;
public:
 bool begin();
 float yawRate(); // radians/sec; positive CCW
};
