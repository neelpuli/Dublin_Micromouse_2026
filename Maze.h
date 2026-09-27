#pragma once
#include <array>
#include <vector>
#include <queue>
#include <stdexcept>
#include <algorithm>

// Coordinates: (0,0) southwest, heading north initially.
enum Dir { N=0, E=1, S=2, W=3 };
inline Dir rotate(Dir d,int delta){return Dir((int(d)+delta+4)%4);}
struct Pos {int x,y; bool operator==(Pos b) const{return x==b.x&&y==b.y;}};
struct Move {char kind; int count;}; // 'F', 'L', 'R'

class Maze {
public:
 static constexpr int MAX=16, INF=100000;
 struct Cell {unsigned char known=0, wall=0;};
 int width,height;
 std::array<std::array<Cell,MAX>,MAX> cells{};
 explicit Maze(int w=16,int h=16):width(w),height(h){
  if(w<2||h<2||w>MAX||h>MAX) throw std::invalid_argument("maze dimensions");
  for(int x=0;x<w;x++) for(int y=0;y<h;y++) for(int d=0;d<4;d++){
   Pos p=next({x,y},Dir(d));if(!inside(p)) set({x,y},Dir(d),true);
  }
 }
 bool inside(Pos p) const{return p.x>=0&&p.x<width&&p.y>=0&&p.y<height;}
 static Pos next(Pos p,Dir d){static constexpr int dx[]={0,1,0,-1},dy[]={1,0,-1,0};return {p.x+dx[d],p.y+dy[d]};}
 bool known(Pos p,Dir d) const{return cells[p.x][p.y].known&(1<<d);}
 bool wall(Pos p,Dir d) const{return cells[p.x][p.y].wall&(1<<d);}
 // Returns true if this edge changed. Also updates its neighbor's opposite edge.
 bool set(Pos p,Dir d,bool blocked){
  if(!inside(p))throw std::out_of_range("set");
  bool changed=!known(p,d)||wall(p,d)!=blocked;
  auto update=[&](Pos q,Dir side){auto &c=cells[q.x][q.y];c.known|=1<<side;
   if(blocked)c.wall|=1<<side;else c.wall&=~(1<<side);};
  update(p,d);Pos q=next(p,d);if(inside(q))update(q,rotate(d,2));
  return changed;
 }
 bool passable(Pos p,Dir d,bool unknownOpen) const{
  Pos q=next(p,d);return inside(q)&&(!known(p,d)?unknownOpen:!wall(p,d));
 }
 std::vector<Pos> goals() const{
  std::vector<Pos> g;
  for(int x=(width-1)/2;x<=width/2;x++){
   for(int y=(height-1)/2;y<=height/2;y++)g.push_back({x,y});
  }
  return g;
 }
 using Dist=std::array<std::array<int,MAX>,MAX>;
 Dist flood(const std::vector<Pos>& targets,bool unknownOpen) const{
  Dist dist;for(auto &col:dist)col.fill(INF);std::queue<Pos> q;
  for(Pos p:targets){if(!inside(p))throw std::out_of_range("target");
   if(dist[p.x][p.y]){dist[p.x][p.y]=0;q.push(p);}}
  while(!q.empty()){Pos p=q.front();q.pop();for(int d=0;d<4;d++){
   Dir dir=Dir(d);if(!passable(p,dir,unknownOpen))continue;
   Pos v=next(p,dir);if(dist[v.x][v.y]>dist[p.x][p.y]+1){dist[v.x][v.y]=dist[p.x][p.y]+1;q.push(v);}
  }}return dist;
 }
 // Follow strictly decreasing flood distances. Prefer straight when equally short.
 Dir choose(Pos p,Dir heading,const Dist& dist,bool unknownOpen) const{
  int best=INF,preference=INF;Dir chosen=N;
  for(int d=0;d<4;d++){Dir dir=Dir(d);if(!passable(p,dir,unknownOpen))continue;
   Pos v=next(p,dir);int cost=dist[v.x][v.y];int turns=(dir==heading?0:(dir==rotate(heading,2)?2:1));
   if(cost<best||(cost==best&&turns<preference)){best=cost;preference=turns;chosen=dir;}}
  if(best==INF||best>=dist[p.x][p.y])throw std::runtime_error("no descending path");
  return chosen;
 }
 // Construct route only through confirmed open edges, merging consecutive F moves.
 std::vector<Move> route(Pos from,Dir heading,const std::vector<Pos>& targets) const{
  auto dist=flood(targets,false);if(dist[from.x][from.y]==INF) return {};
  std::vector<Move> result;Pos p=from;
  while(dist[p.x][p.y]){
   Dir dir=choose(p,heading,dist,false);int delta=(int(dir)-int(heading)+4)%4;
   if(delta==1)result.push_back({'R',1});
   else if(delta==3)result.push_back({'L',1});
   else if(delta==2){result.push_back({'R',1});result.push_back({'R',1});}
   if(!result.empty()&&result.back().kind=='F')result.back().count++;
   else result.push_back({'F',1});
   heading=dir;p=next(p,dir);
  }return result;
 }
 bool shortestKnown(Pos from,const std::vector<Pos>& targets) const{
  auto optimistic=flood(targets,true),confirmed=flood(targets,false);
  return confirmed[from.x][from.y]!=INF&&confirmed[from.x][from.y]==optimistic[from.x][from.y];
 }
};
