#include "Maze.h"
#include "MmsRobot.h"
#include <iostream>
#include <vector>

class Mouse {
 RobotIO& io;
 Maze maze;
 Pos pos{0,0};Dir heading=N;
 enum Phase { TO_CENTER, TO_START, SPEED_RUN, DONE } phase=TO_CENTER;
 bool at(Pos p,const std::vector<Pos>& targets){for(Pos t:targets)if(p==t)return true;return false;}
 void rotateTo(Dir d){int delta=(int(d)-int(heading)+4)%4;
  if(delta==1)io.turnRight();else if(delta==3)io.turnLeft();
  else if(delta==2){io.turnRight();io.turnRight();}heading=d;}
 bool sense(){bool changed=false;
  // The rear edge was crossed safely and is already recorded as open.
  bool readings[]={io.wallFront(),io.wallRight(),io.wallLeft()};
  Dir dirs[]={heading,rotate(heading,1),rotate(heading,3)};
  const char names[]="nesw";
  for(int i=0;i<3;i++){
   Dir d=dirs[i];bool before=maze.known(pos,d),old=maze.wall(pos,d);
   changed|=maze.set(pos,d,readings[i]);
   if(readings[i]&&(!before||!old))io.markWall(pos.x,pos.y,names[d]);
  }io.markCell(pos.x,pos.y,'g');return changed;
 }
 void follow(const std::vector<Move>& moves){
  for(auto m:moves){if(m.kind=='L'){io.turnLeft();heading=rotate(heading,-1);}
   else if(m.kind=='R'){io.turnRight();heading=rotate(heading,1);}
   else {io.moveForward(m.count);for(int i=0;i<m.count;i++)pos=Maze::next(pos,heading);}}
 }
public:
 explicit Mouse(RobotIO& robot,int w,int h):io(robot),maze(w,h){}
 void run(){
  const auto center=maze.goals();const std::vector<Pos> start={{0,0}};
  for(;;){
   if(io.wasReset()){
    // Simulator reset teleports to start. Preserve the learned map; re-localize.
    io.ackReset();pos={0,0};heading=N;phase=TO_CENTER;
    std::cerr<<"Reset: preserved known walls; restarting exploration\n";
   }
   if(phase==DONE)return;
   if(phase==SPEED_RUN){
    // Unknown edges are forbidden. Only run when a confirmed path exists.
    auto moves=maze.route(pos,heading,center);
    if(moves.empty()&&!at(pos,center)){phase=TO_CENTER;continue;}
    std::cerr<<"Speed-run commands:";
    for(auto m:moves)std::cerr<<' '<<m.kind<<(m.kind=='F'?std::to_string(m.count):"");
    std::cerr<<'\n';follow(moves);phase=DONE;continue;
   }
   const auto& targets=(phase==TO_CENTER?center:start);
   if(at(pos,targets)){
    if(phase==TO_CENTER){phase=TO_START;std::cerr<<"Center reached; returning\n";}
    else {phase=maze.shortestKnown({0,0},center)?SPEED_RUN:TO_CENTER;
     std::cerr<<(phase==SPEED_RUN?"Confirmed shortest route; speed run\n":"Explore again\n");}
    continue;
   }
   bool changed=sense();(void)changed; // Re-flood after every sensing cycle, including newly discovered walls.
   auto dist=maze.flood(targets,true);
   if(dist[pos.x][pos.y]==Maze::INF){std::cerr<<"No route to target\n";return;}
   Dir next;
   try{next=maze.choose(pos,heading,dist,true);}
   catch(const std::exception& e){std::cerr<<e.what()<<'\n';return;}
   rotateTo(next);
   // Before advancing, check front again; never deliberately enter a sensed wall.
   if(io.wallFront()){
    maze.set(pos,heading,true);io.markWall(pos.x,pos.y,"nesw"[heading]);continue;
   }
   io.moveForward(1);maze.set(pos,heading,false);pos=Maze::next(pos,heading);
  }
 }
};
int main(){MmsRobot robot;try{Mouse mouse(robot,API::mazeWidth(),API::mazeHeight());mouse.run();}
 catch(const std::exception& e){std::cerr<<"Fatal: "<<e.what()<<'\n';return 1;}}
