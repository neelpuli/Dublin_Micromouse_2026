#include "Sensors.h"
#include "Config.h"

// Same start-up sequence as the Debug 06 sketch that worked on this robot:
// all sensors asleep, then wake one at a time and move each to its own address.
bool Sensors::startOne(VL53L0X& s, int xshut, uint8_t addr, const char* name) {
  digitalWrite(xshut, HIGH);
  delay(10);
  s.setTimeout(100);
  if (!s.init()) {
    digitalWrite(xshut, LOW);
    Serial.printf("  ToF %s FAIL (check XSHUT GPIO%d)\n", name, xshut);
    return false;
  }
  if (addr != 0x29) s.setAddress(addr);
  s.startContinuous();
  Serial.printf("  ToF %s PASS at 0x%02X\n", name, addr);
  return true;
}

bool Sensors::begin() {
  pinMode(X_LEFT, OUTPUT);  digitalWrite(X_LEFT, LOW);
  pinMode(X_FRONT, OUTPUT); digitalWrite(X_FRONT, LOW);
  pinMode(X_RIGHT, OUTPUT); digitalWrite(X_RIGHT, LOW);
  delay(10);
  bool okL = startOne(l, X_LEFT, 0x30, "L");
  bool okF = startOne(f, X_FRONT, 0x31, "F");
  bool okR = startOne(r, X_RIGHT, 0x29, "R");
  return okL && okF && okR;
}

int Sensors::read(VL53L0X& s) {
  uint16_t mm = s.readRangeContinuousMillimeters();
  if (s.timeoutOccurred()) return -1;       // real problem (wiring / bus)
  if (mm >= MAX_VALID_MM) return NO_WALL_MM; // out of range = open, NOT an error
  return mm;
}
