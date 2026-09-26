// ======================= Dublin Micromouse 2026 =======================
// Serial Monitor at 115200. Type a letter and press Enter:
//   s = show sensors, encoders, gyro       z = zero the encoder counts
//   f = drive ONE cell forward             l / r = turn left / right 90
//   g = GO: solve the maze                 x = stop
// Robot must be STILL for ~1 s after power-up (gyro calibration).
// Place it in the start cell facing the opening, outer wall on its LEFT.
// ======================================================================
#include <Arduino.h>
#include "Mouse.h"
#include "Encoders.h"

ESP32Robot robot;
Mouse mouse(robot);
bool running = false;

void help() {
  Serial.println("Commands: s=status z=zero enc f=1 cell l=left r=right g=GO x=stop");
}

void setup() {
  Serial.begin(115200);
  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 2000) {}
  Serial.println("\nMicromouse starting...");
  if (robot.begin()) Serial.println("READY");
  else Serial.println("NOT READY - fix the FAIL above, then press RESET");
  help();
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r' || c == ' ') return;
    if (c == 'x') { running = false; robot.stop(); Serial.println("STOPPED"); return; }
    if (!robot.isReady()) { Serial.println("Not ready - press RESET"); return; }
    if (running) { Serial.println("Running - send x to stop first"); return; }
    robot.clearFault();
    switch (c) {
      case 's': robot.printStatus(); break;
      case 'z': Encoders::reset(); Serial.println("Encoders zeroed"); break;
      case 'f': robot.moveForward(1); robot.printStatus(); break;
      case 'l': robot.turnLeft();  Serial.println("Turned left"); break;
      case 'r': robot.turnRight(); Serial.println("Turned right"); break;
      case 'g': running = true; Serial.println("GO!"); break;
      default: help();
    }
  }
  if (running && !robot.failed()) mouse.step();
}
