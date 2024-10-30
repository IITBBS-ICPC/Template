#include "floor_sum.h"
// take inEq of the for ax+by<=c
struct inEq{
    int a,b,c;
    inEq(int _a,int _b,int _c):
        a(_a),b(_b),c(_c){}
};

inline pair<vector<inEq>,vector<int>>
 filterInequalities(vector<inEq> inq,
  int min_x, int max_x){
    vector<inEq> resInq;
    vector<int> pos;
    for(int i=0;i<inq.size();++i){
        int cur = min_x;
        auto [a,b,c] = inq[i];
        while(!resInq.empty()){
            auto [na,nb,nc] = resInq.back();
            if((a*1ll*nb)==(b*1ll*na)){
                resInq.pop_back();
                pos.pop_back();
            }
            else{
                ll num = c*1ll*nb-nc*1ll*b;
                ll den = na*1ll*b-a*1ll*nb;
                if(den<0){
                    den = -den;
                    num = -num;
                }
                ll cur_x = (num<=0)?(num/den):((num+den-1)/den);
                if(cur_x<=pos.back()){
                    resInq.pop_back();
                    pos.pop_back();
                }
                else{
                    cur = (cur_x>max_x)?max_x:cur_x;
                    break;
                }
            }
        }
        resInq.push_back(inq[i]);
        pos.push_back(cur);
    }
    pos.push_back(max_x+1);
    return mp(resInq, pos);
}

// find integer points in the intersection
// -1 represents infinite
ll integerInequalitySolver(vector<inEq> q){
    int min_x = INT_MIN;
    int max_x = INT_MAX-1;
    vector<inEq> leq,geq;
    bool pos_x = false;
    bool neg_x = false;
    for(auto cur:q){
        pos_x |= (cur.a>0);
        neg_x |= (cur.a<0);
        if(cur.b>0){
            leq.push_back(
                inEq(-cur.a,cur.b,cur.c));
        }
        else if(cur.b<0){
            geq.push_back(
                inEq(cur.a,-cur.b,-cur.c));
        }
        else{
            if(cur.a==0){
                if(cur.c<0)return 0;
            }
            else{
                if(cur.a<0){
                    cur.c = -cur.c;
                    cur.a = -cur.a;
                    if(cur.c<0){
                        min_x = max(min_x,cur.c/cur.a);
                    }
                    else{
                        min_x = 
                          max(min_x,(cur.c+cur.a-1)/cur.a);
                    }
                }
                else{
                    if(cur.c>=0){
                        max_x = min(max_x,cur.c/cur.a);
                    }
                    else{
                        max_x = 
                          min(max_x,(cur.c-cur.a+1)/cur.a);
                    }
                }
            }
        }
    }
    if(max_x<min_x)return 0;
    if(geq.empty()||leq.empty())return -1;
    sort(geq.begin(),geq.end(),[](auto inl,auto inr){
        return mp(inl.a*1ll*inr.b,inl.c*1ll*inr.b)<
                mp(inr.a*1ll*inl.b,inr.c*1ll*inl.b);
    });
    sort(leq.begin(),leq.end(),[](auto inl,auto inr){
        return mp(inl.a*1ll*inr.b,inl.c*1ll*inr.b)>
                mp(inr.a*1ll*inl.b,inr.c*1ll*inl.b);
    });
    auto [G, pG] = filterInequalities(geq,min_x,max_x);
    auto [L, pL] = filterInequalities(leq,min_x,max_x);
    if(min_x==INT_MIN){
        auto [la,lb,lc] = L[0];
        auto [ga,gb,gc] = G[0];
        ll nL = la*1ll*gb;
        ll nG = ga*1ll*lb;
        if(nL<nG)return -1;
        if(nL==nG){
            ll _c = lc*1ll*gb-lb*1ll*gc;
            ll _b = lb*1ll*gb;
            if(_c>=_b)return -1;
            if(_c>=0){
                ll cur = FloorSumAP(la,lc,lb,_b)
                        - FloorSumAP(ga,gc-1,gb,_b);
                if(cur>0)return -1;
            }
        }
    }
    if(max_x==(INT_MAX-1)){
        auto [la,lb,lc] = L.back();
        auto [ga,gb,gc] = G.back();
        ll nL = la*1ll*gb;
        ll nG = ga*1ll*lb;
        if(nL>nG)return -1;
        if(nL==nG){
            ll _c = lc*1ll*gb-lb*1ll*gc;
            ll _b = lb*1ll*gb;
            if(_c>=_b)return -1;
            if(_c>=0){
                ll cur = FloorSumAP(la,lc,lb,_b)
                        - FloorSumAP(ga,gc-1,gb,_b);
                if(cur>0)return -1;
            }
        }
    }
    ll ans = 0;
    for(int cur=min_x,lit=0,git=0;cur<=max_x;){
        if(pL[lit+1]==cur)++lit;
        if(pG[git+1]==cur)++git;
        int ncur = min(pL[lit+1],pG[git+1])-1;
        auto [la,lb,lc] = L[lit];
        auto [ga,gb,gc] = G[git];
        ll ln = la*1ll*gb;
        ll gn = ga*1ll*lb;
        ll diffc = lc*1ll*gb-gc*1ll*lb;
        if(ln==gn){
            if((lc*1ll*gb)>=(lb*1ll*gc)){
                ans+=FloorSumAP(la,lc,lb,cur,ncur);
                ans-=FloorSumAP(ga,gc-1,gb,cur,ncur);
            }
        }
        else if(ln>gn){
            ll den = ln-gn;
            ll num = -diffc;
            ll mid = (num<=0)?(num/den):((num+den-1)/den);
            if(mid<=ncur){
                if(cur<mid)cur=mid;
                ans+=FloorSumAP(la,lc,lb,cur,ncur);
                ans-=FloorSumAP(ga,gc-1,gb,cur,ncur);
            }
        }
        else{
            ll den = gn-ln;
            ll num = diffc;
            ll mid = (num<0)?((num-den+1)/den):(num/den);
            if(mid>=cur){
                int rcur = (mid<ncur)?mid:ncur;
                ans+=FloorSumAP(la,lc,lb,cur,rcur);
                ans-=FloorSumAP(ga,gc-1,gb,cur,rcur);
            }
        }
        cur = ncur+1;
    }

    return ans;
}