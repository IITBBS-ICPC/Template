// Yosupo Problem Link : https://judge.yosupo.jp/problem/bitwise_xor_convolution
// Submission Link : https://judge.yosupo.jp/submission/227588

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
const int mod = 998244353;
inline int normalise(int x){
    if(x>=mod)x-=mod;
    return x;
}
constexpr u32 get_inv(){
    u32 res = mod;
    for(int i=0;i<4;++i)res *= (2-mod*res);
    return -res;
}
const u32 inv = get_inv();
const u64 mod64 = mod;
int reduce(u64 x){
    u32 m = x*inv;
    m = (x+m*mod64)>>32;
    if(m>=mod)m-=mod;
    return m;
}
void mulxor(vector<int> &a, int inv=false) {
    int n = a.size();
    for(int len=2,hlen = 1;len<=n;hlen<<=1,len<<=1) {
        for(int i=0;i<n;i+=len) {
            for(int j=0;j<hlen;++j) {
                int str = a[i+j];
                a[i+j] = normalise(a[i+j] + a[i+j+hlen]);
                a[i+j+hlen] = normalise(str - a[i+j+hlen]+mod);
            }
        }
    }
    if(inv) for(int i=0;i<n;++i) {
        u64 x = a[i];
        a[i] = reduce((x<<32)/n);
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
    mulxor(a), mulxor(b);
    for(int i=0;i<n;++i){
        a[i] = (a[i]*1ll*b[i])%mod;
    }
    mulxor(a,true);
    cout<<a<<"\n";
	return 0;
}