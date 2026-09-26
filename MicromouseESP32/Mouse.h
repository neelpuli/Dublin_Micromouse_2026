#pragma once
#include "Maze.h"
#include "ESP32Robot.h"
class Mouse {
 ESP32Robot &io;Maze maze;Pos pos{0,0};Dir heading=N;
 enum Phase{TO_CENTER,TO_START,SPEED_RUN,DONE} phase=TO_CENTER;
 bool at(Pos p,const std::vector<Pos>& v){for(auto q:v)if(q==p)return true;return false;}
 void rotateTo(Dir d){int delta=(int(d)-int(heading)+4)%4;if(delta==1)io.turnRight();else if(delta==3)io.turnLeft();else if(delta==2){io.turnRight();if(!io.failed())io.turnRight();}if(!io.failed())heading=d;}
 void sense(){bool walls[]={io.wallFront(),io.wallRight(),io.wallLeft()};if(io.failed())return;Dir dirs[]={heading,rotate(heading,1),rotate(heading,3)};for(int i=0;i<3;i++)maze.set(pos,dirs[i],walls[i]);}
 void follow(const std::vector<Move>& moves){for(auto m:moves){if(io.failed())return;if(m.kind=='L'){io.turnLeft();if(!io.failed())heading=rotate(heading,-1);}else if(m.kind=='R'){io.turnRight();if(!io.failed())heading=rotate(heading,1);}else{io.moveForward(m.count);if(!io.failed())for(int i=0;i<m.count;i++)pos=Maze::next(pos,heading);}}}
public:
 explicit Mouse(ESP32Robot& r):io(r),maze(16,16){}
 // One navigation step per loop; blocking movement with timeouts.
 void step(){if(io.failed()||phase==DONE)return;const auto center=maze.goals();const std::vector<Pos> start={{0,0}};
  if(phase==SPEED_RUN){auto moves=maze.route(pos,heading,center);if(moves.empty()&&!at(pos,center)){phase=TO_CENTER;return;}Serial.println("Executing confirmed route");follow(moves);if(!io.failed())phase=DONE;return;}
  const auto &target=phase==TO_CENTER?center:start;
  if(at(pos,target)){if(phase==TO_CENTER){phase=TO_START;Serial.println("Centre reached");}else{phase=maze.shortestKnown({0,0},center)?SPEED_RUN:TO_CENTER;Serial.println(phase==SPEED_RUN?"Confirmed route":"Exploring again");}return;}
  sense();if(io.failed())return;
  auto dist=maze.flood(target,true);if(dist[pos.x][pos.y]==Maze::INF){io.halt("No optimistic route");return;}
  Dir next;try{next=maze.choose(pos,heading,dist,true);}catch(...){io.halt("No descending path");return;}
  rotateTo(next);if(io.failed())return;
  if(io.wallFront()){if(!io.failed())maze.set(pos,heading,true);return;}if(io.failed())return;
  io.moveForward(1);if(io.failed())return;maze.set(pos,heading,false);pos=Maze::next(pos,heading);
 }
};
