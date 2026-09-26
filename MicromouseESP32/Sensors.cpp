#include "Sensors.h"
#include "Config.h"
bool Sensors::begin(){
 // IMPORTANT: XSHUT pullups must not defeat shutdown; check your breakouts.
 pinMode(X_LEFT,OUTPUT);pinMode(X_FRONT,OUTPUT);pinMode(X_RIGHT,OUTPUT);
 digitalWrite(X_LEFT,LOW);digitalWrite(X_FRONT,LOW);digitalWrite(X_RIGHT,LOW);delay(30);
 digitalWrite(X_LEFT,HIGH);delay(30);if(!l.begin(0x30,false,&Wire))return false;
 digitalWrite(X_FRONT,HIGH);delay(30);if(!f.begin(0x31,false,&Wire))return false;
 digitalWrite(X_RIGHT,HIGH);delay(30);if(!r.begin(0x32,false,&Wire))return false;
 return true;
}
int Sensors::read(Adafruit_VL53L0X &s){VL53L0X_RangingMeasurementData_t m;s.rangingTest(&m,false);return m.RangeStatus==4?-1:static_cast<int>(m.RangeMilliMeter);}
