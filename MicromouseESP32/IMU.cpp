#include "IMU.h"

bool IMU::present(uint8_t a) {
  Wire.beginTransmission(a);
  return Wire.endTransmission() == 0;
}

void IMU::writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

float IMU::readDps() {
  Wire.beginTransmission(addr);
  Wire.write(0x47);                 // GYRO_ZOUT_H
  Wire.endTransmission(false);
  if (Wire.requestFrom(addr, (uint8_t)2) != 2) return 0;
  uint8_t hi = Wire.read();
  uint8_t lo = Wire.read();
  int16_t raw = (int16_t)((hi << 8) | lo);
  return raw / 65.5f;               // +/-500 dps range
}

bool IMU::begin() {
  if (present(0x68)) addr = 0x68;
  else if (present(0x69)) addr = 0x69;
  else { Serial.println("  IMU FAIL (not found at 0x68/0x69)"); return false; }
  writeReg(0x6B, 0x01);  // wake up
  delay(100);
  writeReg(0x1B, 0x08);  // +/-500 dps
  writeReg(0x1A, 0x04);  // ~21 Hz filter, reduces motor vibration
  delay(50);
  Serial.println("  IMU PASS - calibrating gyro, keep STILL...");
  double sum = 0;
  for (int i = 0; i < 500; i++) { sum += readDps(); delay(2); }
  biasDps = sum / 500.0;
  return true;
}

float IMU::yawRate() {
  return (readDps() - biasDps) * (PI / 180.0f);
}
