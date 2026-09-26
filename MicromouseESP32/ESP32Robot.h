#pragma once
#include "RobotIO.h"
#include "Sensors.h"
#include "IMU.h"

class ESP32Robot : public RobotIO {
  Sensors sensors;
  IMU imu;
  bool fault = false;
  bool ready = false;
  bool readWall(int d);
  void turn(bool left);
public:
  bool begin();
  bool failed() const { return fault; }
  bool isReady() const { return ready; }
  void clearFault() { if (ready) fault = false; }
  void halt(const char* why);
  void stop();
  void printStatus();
  bool wallLeft() override  { return readWall(0); }
  bool wallFront() override { return readWall(1); }
  bool wallRight() override { return readWall(2); }
  void moveForward(int n) override;
  void turnLeft() override  { turn(true); }
  void turnRight() override { turn(false); }
};
