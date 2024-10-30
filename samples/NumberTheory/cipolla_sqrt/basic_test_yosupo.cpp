// Yosupo Problem Link : https://judge.yosupo.jp/problem/sqrt_mod
// Submission Link : https://judge.yosupo.jp/submission/227516

#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")
#include<bits/stdc++.h>
using namespace std;
template<typename T>istream & operator >> (istream &in, vector<T> &a){for(auto &x:a)in>>x;return in;}
template<typename T>ostream & operator << (ostream &out, vector<T> &a){for(auto &x:a)out<<x<<" ";return out;}
template<typename T,typename K>istream & operator >> (istream &in,  pair<T,K> &a){in>>a.F>>a.S;return in;}
template<typename T,typename K>ostream & operator << (ostream &out, pair<T,K> &a){out<<a.F<<" "<<a.S;return out;}
#define F   first
#define S   second 
#define ll  long long
class cipolla_sqrt{
 private:
    const int phim,phi,phihalf,phalf,p;
 public:
    cipolla_sqrt(int p):
        p(p),phi(p-1),phim(p-2),
        phihalf(p>>1),phalf((p+1)>>1){}
    int pow(int a,int po){
        int res = 1;
        for(;po;po>>=1,a=normalise(a*1ll*a))
        if(po&1){
            res=normalise(a*1ll*res);
        }
        return res;
    }
    int normalise(ll x){
        if(x>=p){
            x-=p;if(x>=p)x%=p;return x;
        }
        return x;
    }
    int get(int x){
        int a = 0,b;
        while(
            pow(b = normalise(a*1ll*a-x+p),
                phihalf)==1
        )++a;
        int po = phalf;
        pair<int,int> v = {a,1}, res = {1,1};
        while(po){
            if(po&1){
                int temp = normalise(res.S*1ll*v.S);
                res.S = normalise((v.F*1ll*res.S)
                        +(res.F*1ll*v.S));
                res.F = normalise((v.F*1ll*res.F)
                        +(b*1ll*temp));
            }
            po>>=1;
            int temp = normalise(v.S*1ll*v.S);
            v.S = normalise((v.F*1ll*v.S)
                                +(v.F*1ll*v.S));
            v.F = normalise((v.F*1ll*v.F)
                                +(b*1ll*temp));
        }
        if(res.F==1)res.F = p - res.F;
        return res.F;
    }
};

int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin>>t;
    while(t--){
        int y,p;
        cin>>y>>p;
        cipolla_sqrt c(p);
        if(!y)cout<<y<<"\n";
        else if(c.pow(y,p>>1)==1){
            cout<<c.get(y)<<"\n";
        }   
        else cout<<"-1\n";
    }
	return 0;
}