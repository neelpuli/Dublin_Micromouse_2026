#pragma once
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
class Sensors {
 Adafruit_VL53L0X l,f,r;
 int read(Adafruit_VL53L0X &sensor);
public:
 bool begin();
 int left(){return read(l);}
 int front(){return read(f);}
 int right(){return read(r);}
};
