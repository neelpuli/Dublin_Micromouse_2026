#include "ESP32Robot.h"
#include "Config.h"
#include "Motors.h"
#include "Encoders.h"
#include <math.h>
bool ESP32Robot::begin(){Motors::begin();Encoders::begin();Wire.begin(SDA_PIN,SCL_PIN);Wire.setClock(100000);if(!sensors.begin()){halt("VL53L0X init failed");return false;}if(!imu.begin()){halt("MPU6050 init failed");return false;}return true;}
void ESP32Robot::halt(const char* why){Motors::stop();fault=true;Serial.print("HALT: ");Serial.println(why);}
bool ESP32Robot::readWall(int d){if(fault)return true;int mm=d==0?sensors.left():d==1?sensors.front():sensors.right();if(mm<=0){halt("Invalid range reading");return true;}return mm<WALL_MM;}
void ESP32Robot::moveForward(int n){
 if(fault||n<=0)return;
 // Intentionally one-cell segments until alignment and braking are calibrated.
 const long ticks=lroundf(CELL_MM/(PI*WHEEL_DIAMETER_MM)*COUNTS_PER_WHEEL_REV);
 for(int cell=0;cell<n&&!fault;cell++){
  Encoders::reset();unsigned long start=millis();
  while(!fault){
   long l=labs(Encoders::left()),r=labs(Encoders::right());
   if((l+r)/2>=ticks)break;
   if(millis()-start>MOVE_TIMEOUT_MS){halt("Forward timeout");break;}
   int frontMM=sensors.front();
   if(frontMM<=0){halt("Invalid range reading");break;}
   if(frontMM<FRONT_STOP_MM){halt("Obstacle too close");break;}
   // Basic encoder matching; replace with tuned PID after calibration.
   long error=l-r;
   int correction=constrain(static_cast<int>(error/4),-35,35);
   Motors::drive(MOTOR_PWM-correction,MOTOR_PWM+correction);
   delay(5);
  }
  Motors::stop();delay(120);
 }
}
void ESP32Robot::turn(bool left){
 if(fault)return;
 unsigned long start=millis(),prev=micros();float angle=0;
 // Verify gyro Z sign and motor direction with wheels lifted first.
 while(fabsf(angle)<PI/2.0f){
  if(millis()-start>TURN_TIMEOUT_MS){halt("Turn timeout");return;}
  unsigned long now=micros();float dt=(now-prev)/1000000.0f;prev=now;
  float rate=imu.yawRate();angle+=rate*dt;
  Motors::drive(left?-MOTOR_PWM:MOTOR_PWM,left?MOTOR_PWM:-MOTOR_PWM);
  delay(4);
 }
 Motors::stop();delay(150);
}
