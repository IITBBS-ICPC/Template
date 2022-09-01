#include<bits/stdc++.h>
#define ll           long long int
#define pb           push_back
#define F            first
#define S            second
#define mk           make_pair
const int mod = 1e9+7;
const int P1 = 880803841;
const int P2 = 897581057;
const int P3 = 998244353;
using namespace std;
template<typename T>istream & operator >> (istream &in, vector<T> &a){for(auto &x:a)in>>x;return in;}
template<typename T>ostream & operator << (ostream &out, vector<T> &a){for(auto &x:a)out<<x<<" ";return out;}
template<typename T,typename K>istream & operator >> (istream &in,  pair<T,K> &a){in>>a.F>>a.S;return in;}
template<typename T,typename K>ostream & operator << (ostream &out, pair<T,K> &a){out<<a.F<<" "<<a.S;return out;}

template<int p>
class helper{
    private:
        static int const phim = p-2;
        static int const phi = p-1;
        static int const phihalf = phi>>1;
        static int const phalf = (p+1)>>1;
    public:
        static int pow(int a,int po=phim){int res = 1;for(;po;po>>=1,a=normalise(a*1ll*a))if(po&1)res=normalise(a*1ll*res);return res;}
        static int normalise(ll x){
            if(x>=p)x-=p;
            if(x>=p)x%=p;
            return x;
        }
        static int cipollasqrt(int x){
            int a = 0,b;
            while(pow(b = normalise(a*1ll*a-x+p), phihalf)==1)++a;
            int po = phalf;
            pair<int,int> v = {a,1};
            pair<int,int> res = {1,1};
            while(po){
                if(po&1){
                    int temp = normalise(res.S*1ll*v.S);
                    res.S = normalise((v.F*1ll*res.S)+(res.F*1ll*v.S));
                    res.F = normalise((v.F*1ll*res.F)+(b*1ll*temp));
                }
                po>>=1;
                int temp = normalise(v.S*1ll*v.S);
                v.S = normalise((v.F*1ll*v.S)+(v.F*1ll*v.S));
                v.F = normalise((v.F*1ll*v.F)+(b*1ll*temp));
            }
            if(res.F==1)res.F = p - res.F;
            return res.F;
        }
};

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
    static const __uint32_t N2 = find2inv(mod);
    static const __uint64_t modR = mod;
    constexpr static const __uint32_t Rmod = (1ll<<32)%mod;
    constexpr static const __uint32_t R2mod = (Rmod*1ll*Rmod) %mod;
    static __uint32_t mul(__uint64_t t){t = (t+(modR*(__uint32(t)*N2)))>>32;if(t>=mod)t-=mod;return t;}
    Montgomery(int val):val(mul(val*1ll*R2mod)){}
    Montgomery& operator += (const Montgomery& other){val+=other.val;if(val>=mod)val-=mod;return *this;}
    Montgomery& operator -= (const Montgomery& other){val+=(mod-other.val);if(val>=mod)val-=mod;return *this;}
    Montgomery& operator *= (const Montgomery& other){
        val = mul(val*1ll*other.val);
        return *this;
    }
    int get(){return mul(val);}
};
int gcd(int a, int b, int& x, int& y) {
    x = 1, y = 0;
    int x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1) {
        int q = a1 / b1;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a1, b1) = make_tuple(b1, a1 - q * b1);
    }
    return a1;
}

template<int p>
class mod_complex{
    private:
        int x,y;
    public:
        int base;
        mod_complex(int base,int x=0,int y=0):x(x),y(y){}
        mod_complex& operator*=(const mod_complex &other){
            int temp = helper<p>::normalise(other.y*1ll*y);
            y = helper<p>::normalise((other.y*1ll*x)+(other.x*1ll*y));
            x = helper<p>::normalise((other.x*1ll*x)+(base*1ll*temp));
        }
        static mod_complex pow(mod_complex a,int po){
            mod_complex res(a.base,1);
            while(po>>1){
                if(po&1)res*=a;
                po>>=1;
                a*=a;
            }
            return res;
        }
        int real(){
            return x;
        }
        int imag(){
            return y;
        }
};
int main(){
return 0;}
