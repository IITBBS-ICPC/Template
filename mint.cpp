#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#include<bits/stdc++.h>
#define ll           long long int
#define pb           push_back
#define F            first
#define S            second
#define mk           make_pair
#define __uint32_t uint32_t
#define __uint64_t uint64_t


using namespace std;

template<int mod>
class Montgomery{
private:
        __uint32_t val;
public:
    constexpr static __uint32_t find2inv(){
        __uint32_t inv = mod;
        for(int i=0;i<4;++i)inv *=(2-inv*mod);
        return -inv;
    }
    static const __uint32_t N2 = find2inv();
    static const __uint64_t modR = mod;
    constexpr static const __uint32_t Rmod = (1ull<<32)%mod;
    constexpr static const __uint64_t R2mod = (Rmod*1ull*Rmod) %mod;
    constexpr static const __uint64_t R3mod = (R2mod*1ull*Rmod) %mod;
    static __uint32_t mul(__uint64_t t){t = (t+(modR*(__uint32_t(t)*N2)))>>32;if(t>=mod)t-=mod;return t;}
    static __uint32_t modinverse(int x){
        int a = mod, b = x, u = 0, v = 1,temp = 0;
        while(b){
            int q = a/b;
            a-=(b*q);swap(a,b);
            u-=(v*q);swap(u,v);
        }
        assert(a==1);
        if(u<0)u+=mod;
        if(u>=mod)u-=mod;
        return u;
    }
    static const __uint32_t normalise(int v){
        if(v<0){
            if(v<-mod)v%mod;
            v+=mod;
        }
        else if(v>=mod)v%=mod;
        return v;
    }
    Montgomery():val(0){}
    Montgomery(int v):val(mul(normalise(v)*R2mod)){}
    Montgomery(__uint32_t val):val(val){}
    Montgomery& operator += (const Montgomery& other){val+=other.val;if(val>=mod)val-=mod;return *this;}
    Montgomery& operator -= (const Montgomery& other){val+=(mod-other.val);if(val>=mod)val-=mod;return *this;}
    Montgomery& operator *= (const Montgomery& other){
        val = mul(val*__uint64_t(other.val));
        return *this;
    }
    Montgomery operator + (const Montgomery& other){
        __uint32_t temp = val+other.val;
        if(temp>=mod)temp-=mod;
        return Montgomery<mod> (temp);
    }
    Montgomery operator - (const Montgomery& other){
        __uint32_t temp = val-other.val+mod;
        if(temp>=mod)temp-=mod;
        return Montgomery<mod> (temp);
    }
    Montgomery operator * (const Montgomery& other){
        return Montgomery<mod> (mul(val*__uint64_t(other.val)));;
    }
    Montgomery& operator /= (const Montgomery& other){
        __uint64_t inv = modinverse(other.val);
        val = mul(R3mod*mul(val*inv));
        return *this;
    }
    Montgomery operator / (const Montgomery& other){
        __uint64_t inv = modinverse(other.val);
        return Montgomery<mod> (mul(R3mod*mul(val*inv)));
    }
    Montgomery operator - (){
        return Montgomery()-*this;
    }
    Montgomery operator + (){
        return *this;
    }
    int get(){return mul(val);}

    template <int m>
    friend bool operator==(const Montgomery<m>& lhs, const int& rhs);

    template <int m>
    friend std::istream& operator>>(std::istream& stream, Montgomery<m>& number);
    template <int m>
    friend std::ostream& operator<<(std::ostream& stream, Montgomery<m>& number);
};

template<int mod> bool operator == (const Montgomery<mod>& lhs, const int& rhs){return lhs.get()==rhs;}
template<int mod> bool operator == (const int&rhs,const Montgomery<mod> &lhs){return lhs==rhs;}
template<int mod> Montgomery<mod> operator + (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)+=a;}
template<int mod> Montgomery<mod> operator - (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)-=a;}
template<int mod> Montgomery<mod> operator * (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)*=a;}
template<int mod> Montgomery<mod> operator / (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)/=a;}

template<typename T> T pow(T a,int p){T res(1);while(p){if(p&1)res*=a;p>>=1;a*=a;};return res;}

const int mod = 1e9+7;
using mint = Montgomery<mod>;
vector<mint> fac,facinv,inv;
void pre(int n){
    ++n;
    fac.resize(n);
    facinv.resize(n);
    inv.resize(n);
    fac[0]=fac[1]=facinv[0]=facinv[1]=inv[0]=inv[1]=mint::Rmod;
    for(int i = 2; i<n ; ++i){
        fac[i] = fac[i-1]*i;
        inv[i] = -((mod/i)*inv[mod%i]);
        facinv[i] = facinv[i-1]*i;
    }
}

template<typename T>istream & operator >> (istream &in, vector<T> &a){for(auto &x:a)in>>x;return in;}
template<typename T>ostream & operator << (ostream &out, vector<T> &a){for(auto &x:a)out<<x<<" ";return out;}
template<typename T,typename K>istream & operator >> (istream &in,  pair<T,K> &a){in>>a.F>>a.S;return in;}
template<typename T,typename K>ostream & operator << (ostream &out, pair<T,K> &a){out<<a.F<<" "<<a.S;return out;}
template<int m>istream & operator >> (istream &in, Montgomery<m> &a){int x;in>>x;a.val = mul((Montgomery<m>::R2mod)*x);return in;}
template<int m>ostream & operator << (ostream &out, Montgomery<m> &a){out<<a.get();return out;}

void solve()
{
    mint r(5), q(34);
    mint c = r*q;
    cout << c.get() << endl ;
    int x = -5;
    c=((mint)5)*c;
    c /= -x;
    mint z = 5+c;
    cout << z << endl ;
}
vector<int> check(){return vector<int> ();}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin>>t;
    for(int i=1;i<=t;++i){
        solve();
    }
  return 0;
}
