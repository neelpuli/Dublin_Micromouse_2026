#include <Arduino.h>
#include "Mouse.h"
ESP32Robot robot;
Mouse mouse(robot);
// Safety: robot does NOT start moving until you send 'g' in Serial Monitor.
bool started=false;
void setup(){Serial.begin(115200);delay(1200);Serial.println("Micromouse hardware init");if(robot.begin())Serial.println("Ready: send g to begin (only after calibration)");}
void loop(){if(robot.failed())return;if(!started){if(Serial.available()&&Serial.read()=='g')started=true;return;}mouse.step();}
