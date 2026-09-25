#pragma once
// The only layer that must be replaced by the ESP32 motor/sensor implementation.
struct RobotIO {
 virtual ~RobotIO()=default;
 virtual bool wallLeft()=0;
 virtual bool wallFront()=0;
 virtual bool wallRight()=0;
 virtual void moveForward(int n)=0;
 virtual void turnLeft()=0;
 virtual void turnRight()=0;
 virtual bool wasReset(){return false;}
 virtual void ackReset(){}
 virtual void markWall(int,int,char){}
 virtual void markCell(int,int,char){}
};
