#pragma once
#include "RobotIO.h"
#include "API.h"
struct MmsRobot:RobotIO {
 bool wallLeft()override{return API::wallLeft();}
 bool wallFront()override{return API::wallFront();}
 bool wallRight()override{return API::wallRight();}
 void moveForward(int n)override{API::moveForward(n);}
 void turnLeft()override{API::turnLeft();}
 void turnRight()override{API::turnRight();}
 bool wasReset()override{return API::wasReset();}
 void ackReset()override{API::ackReset();}
 void markWall(int x,int y,char d)override{API::setWall(x,y,d);}
 void markCell(int x,int y,char c)override{API::setColor(x,y,c);}
};
