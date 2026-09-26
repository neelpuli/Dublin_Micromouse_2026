#pragma once
#include <Arduino.h>
class Encoders {
public:
 static void begin();
 static void reset();
 static long left();
 static long right();
};
