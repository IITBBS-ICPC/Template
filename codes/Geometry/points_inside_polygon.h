#include "../NumberTheory/inequality_solver.h"

struct pt{
    int x,y;   
    pt(int _x,int _y):x(_x),y(_y){} 
    pt operator - (const pt &other) const{
        return pt(x-other.x,y-other.y);
    }
    pt operator - () const{
        return pt(-x,-y);
    }
};
ll cross(pt a,pt b){
    return a.x*1ll*b.y-a.y*1ll*b.x;
}
struct edge{
    pt s,e;
    bool isCounterClockWise(pt c,bool includeLinear=false){
        return cross(e-s,c-s)>=includeLinear;
    }
};

ll getPointsInsidePolygon(vector<edge> edges, bool includeOnLine = false){
    vector<inEq> inq;
    for(auto [s,e]:edges){
        pt t = e-s;
        inq.push_back(
            inEq(t.x,-t.y,cross(s,t)-1+includeOnLine)
        );
    }
    return integerInequalitySolver(inq);
}