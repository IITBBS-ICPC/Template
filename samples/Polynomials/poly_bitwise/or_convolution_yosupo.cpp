// Yosupo Problem Link : https://judge.yosupo.jp/problem/bitwise_and_convolution
// Submission Link : https://judge.yosupo.jp/submission/227640

// using not property of or and (demorgan) to verify

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
#define u32     uint32_t 
#define u64     uint64_t
#define all(x)  x.begin(),x.end()
const int mod = 998244353;
inline int normalise(int x){
    if(x>=mod)x-=mod;
    return x;
}
void mulor(vector<int> &a, int inv) {
    int n = a.size();
    for(int len=2,hlen=1;len<=n;hlen<<=1,len<<=1) {
        for(int i=0;i<n;i+=len) {
            for(int j=0;j<hlen;++j) {
                int str = a[i+j];
                if(!inv) {
                    a[i+j] = normalise(a[i+j]+a[i+j+hlen]);
                    a[i+j+hlen] = str;   
                } else {
                    a[i+j] = a[i+j+hlen];
                    a[i+j+hlen] = normalise(mod+str-a[i+j+hlen]);  
                }
            }
        }
    }
}

int powmod(int a,int p=mod-2){
    int res = 1;
    while(p){
        if(p&1){
            res = (res*1ll*a)%mod;
        }
        p>>=1;
        a = (a*1ll*a)%mod;
    }
    return res;
}
int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin>>n;
    n = 1<<n;
    vector<int> a(n),b(n);
    cin>>a>>b;
    reverse(all(a));
    reverse(all(b));
    mulor(a,false), mulor(b,false);
    for(int i=0;i<n;++i){
        a[i] = (a[i]*1ll*b[i])%mod;
    }
    mulor(a,true);
    reverse(all(a));
    cout<<a<<"\n";
	return 0;
}