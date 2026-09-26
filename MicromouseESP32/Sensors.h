#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>   // Pololu library - the same one that passed Debug 06

class Sensors {
  VL53L0X l, f, r;
  bool startOne(VL53L0X& s, int xshut, uint8_t addr, const char* name);
  int read(VL53L0X& s);
public:
  bool begin();
  // millimetres; NO_WALL_MM if nothing in range; -1 if the sensor stopped answering
  int left()  { return read(l); }
  int front() { return read(f); }
  int right() { return read(r); }
};
