#include "Motors.h"
#include "Config.h"
#include <Arduino.h>
void Motors::begin(){pinMode(L_DIR,OUTPUT);pinMode(R_DIR,OUTPUT);pinMode(L_PWM,OUTPUT);pinMode(R_PWM,OUTPUT);pinMode(STBY_PIN,OUTPUT);digitalWrite(STBY_PIN,HIGH);stop();}
void Motors::drive(int left,int right){left=constrain(left,-255,255);right=constrain(right,-255,255);digitalWrite(L_DIR,left>=0?HIGH:LOW);digitalWrite(R_DIR,right>=0?HIGH:LOW);analogWrite(L_PWM,abs(left));analogWrite(R_PWM,abs(right));}
void Motors::stop(){analogWrite(L_PWM,0);analogWrite(R_PWM,0);}
