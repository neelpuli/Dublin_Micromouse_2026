#include "IMU.h"
#include <Arduino.h>
bool IMU::begin(){if(!sensor.begin())return false;sensor.setGyroRange(MPU6050_RANGE_250_DEG);sensor.setFilterBandwidth(MPU6050_BAND_21_HZ);delay(100);float sum=0;for(int i=0;i<200;i++){sensors_event_t a,g,t;sensor.getEvent(&a,&g,&t);sum+=g.gyro.z;delay(3);}bias=sum/200.0f;return true;}
float IMU::yawRate(){sensors_event_t a,g,t;sensor.getEvent(&a,&g,&t);return g.gyro.z-bias;}
