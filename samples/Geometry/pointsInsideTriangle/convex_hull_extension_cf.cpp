// https://codeforces.com/gym/104757/problem/C
#include <bits/stdc++.h>
#define ll  long long
#define mp  make_pair
using namespace std;

// f(a,b,c,n) = \sigma ( (a*x + b)/c ) from x=0 to n in 
//O(logn) where value is floor of the value.
// a>=0,b>=0,c>0
ll FloorSumAPPositive(ll a, ll b, 
  ll c, ll n){
  if(!a) return (b / c) * (n + 1);
  if(a >= c or b >= c) return ( ( n * (n + 1) ) / 2) * (a / c) 
  + (n + 1) * (b / c) + FloorSumAPPositive(a % c, b % c, c, n);
  ll m = (a * n + b) / c;
  return m * n - FloorSumAPPositive(c, c - b - 1, a, m - 1);
}

// f(a,b,c,n) = \sigma ( (a*x + b)/c ) from x=0 to n in 
//O(logn) where value is floor of the value.
ll FloorSumAP(ll a,ll b,
  ll c, ll n){
    if(c<0){
      c = -c;
      a = -a;
      b = -b;
    }
    ll _qa = (a>=0)?(a/c):((a-c+1)/c);
    ll _qb = (b>=0)?(b/c):((b-c+1)/c);
    ll _a = a-c*_qa;
    ll _b = b-c*_qb;
    return _qa*((n*(n+1))/2)
            +_qb*(n+1)+FloorSumAPPositive(_a,_b,c,n);
}

// f(a,b,c,n) = \sigma ( (a*x + b)/c ) from x=l to r in 
//O(logn) where value is floor of the value.
ll FloorSumAP(ll a,ll b,
  ll c, ll l, ll r){
    return FloorSumAP(a,b+a*l,c,r-l);
}

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
                        min_x = max(min_x,(cur.c+cur.a-1)/cur.a);
                    }
                }
                else{
                    if(cur.c>=0){
                        max_x = min(max_x,cur.c/cur.a);
                    }
                    else{
                        max_x = min(max_x,(cur.c-cur.a+1)/cur.a);
                    }
                }
            }
        }
    }
    if(max_x<min_x)return 0;
    if(geq.empty()||leq.empty())return -1;
    sort(geq.begin(),geq.end(),[](auto inl,auto inr){
        return mp(inl.a*1ll*inr.b,inl.c*1ll*inr.b)<mp(inr.a*1ll*inl.b,inr.c*1ll*inl.b);
    });
    sort(leq.begin(),leq.end(),[](auto inl,auto inr){
        return mp(inl.a*1ll*inr.b,inl.c*1ll*inr.b)>mp(inr.a*1ll*inl.b,inr.c*1ll*inl.b);
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
struct pt{
    int x,y;   
    pt(int _x,int _y):x(_x),y(_y){}
    pt(){}
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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin>>n;
    vector<pt> points(n);
    for(auto& [x,y]:points)cin>>x>>y;
    vector<edge> edges;
    for(int i=0;i<n;++i){
        pt prev = i?points[i-1]:points.back();
        edge e = {
            .s = prev,
            .e = points[i]
        };
        edges.push_back(e);
    }
    ll ans = 0;
    for(int i=0;i<n;++i){
        edge prev = i?edges[i-1]:edges.back();
        edge next = ((i+1)<n)?edges[i+1]:edges[0];
        edge cur = edges[i];
        swap(cur.s,cur.e);
        ll cur_ans = getPointsInsidePolygon({prev,cur,next});
        if(cur_ans==-1){
            ans = -1;
            break;
        }
        ans+=cur_ans;
    }
    if(ans!=-1)cout<<ans<<"\n";
    else cout<<"infinitely many\n";
}