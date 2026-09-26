#pragma once
class Motors {
public:
 static void begin();
 // Signed -255..255; direction HIGH polarity must be verified per motor.
 static void drive(int left,int right);
 static void stop();
};
