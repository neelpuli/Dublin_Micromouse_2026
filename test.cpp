#include "Maze.h"
#include <cassert>
#include <iostream>
int main(){
 Maze m;auto g=m.goals();assert(g.size()==4);
 auto a=m.flood(g,true);assert(a[0][0]==14);
 assert(m.set({0,0},N,false));assert(m.known({0,1},S));
 assert(!m.set({0,0},N,false));assert(m.set({0,0},N,true));
 assert(m.wall({0,1},S));
 assert(!m.shortestKnown({0,0},g));
 Maze corridor(4,4);for(int x=0;x<4;x++)for(int y=0;y<4;y++)
  for(int d=0;d<4;d++)if(corridor.inside(Maze::next({x,y},Dir(d))))corridor.set({x,y},Dir(d),false);
 assert(corridor.shortestKnown({0,0},corridor.goals()));
 auto r=corridor.route({0,0},N,{{0,3}});assert(r.size()==1&&r[0].kind=='F'&&r[0].count==3);
 std::cout<<"Core tests passed\n";
}
