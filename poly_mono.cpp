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
using namespace std;
template<typename T>istream & operator >> (istream &in, vector<T> &a){for(auto &x:a)in>>x;return in;}
template<typename T>ostream & operator << (ostream &out, vector<T> &a){for(auto &x:a)out<<x<<" ";return out;}
template<typename T,typename K>istream & operator >> (istream &in,  pair<T,K> &a){in>>a.F>>a.S;return in;}
template<typename T,typename K>ostream & operator << (ostream &out, pair<T,K> &a){out<<a.F<<" "<<a.S;return out;}

template<int mod>
constexpr int powmod(int a,int p = mod -2){
    int res = 1;
    while(p){
        if(p&1)res = (res*1ll*a)%mod;
        p>>=1;
        a = (a*1ll*a)%mod;
    }
    return res;
}
template<int max_base,int mod,int primitive>
class Ntt{
    private:
        constexpr static vector<__uint64_t> init_omegas(){
            const int omega = (powmod<mod>(primitive,(mod-1)/max_base)*(1ll<<32))%mod;
            vector<__uint64_t> res(max_base);
            const int m = max_base>>1;
            res[m] = (1ll<<32)%mod;
            for(int i = m+1;i<max_base;++i)res[i] = reduce(res[i-1]*1ll*omega);
            for(int i = m-1;i;--i) res[i] = res[i<<1];
            res[0] = (1ll<<32)%mod;
            return res;
        }
        static const vector<__uint64_t> omegas;
        constexpr static vector<__uint64_t> init_iomegas(){
            const int omega = powmod<mod>(primitive,(mod-1)/max_base);
            const int iomega = (powmod<mod>(omega)*(1ll<<32))%mod;
            vector<__uint64_t> res(max_base,(1ll<<32)%mod);
            const int m = max_base>>1;
            res[m] = (1ll<<32)%mod;
            for(int i = m+1;i<max_base;++i)res[i] = reduce(res[i-1]*1ll*iomega);
            for(int i = m-1;i>=0;--i) res[i] = res[i<<1];
            return res;
        }
        static const vector<__uint64_t> iomegas;
        constexpr static __uint32_t init_N1(){
            __uint32_t inv = N2;
            for(int i=0;i<4;++i)inv*=(2-(N2*inv));
            return -inv;
        }
        static const __uint32_t N1 = init_N1();
        static const __uint64_t N = mod;
        static const __uint32_t N2 = mod;
        static const __uint32_t reduce(__uint64_t x){
            __uint32_t t = (x+(((static_cast<__uint32_t>(x))*N1)*N))>>32;
            if(t>=mod)t-=mod;
            return t;
        }
    public:
        static void fft(vector<int> &a){
            int n = a.size();
            for(int m=n>>1;m;m>>=1){
                auto it_start = omegas.begin()+m;
                auto it_end = it_start+m;
                for(auto l = a.begin();l!=a.end();l+=m){
                    for(auto it = it_start;it!=it_end;++it,++l){
                        int e = *l-l[m];
                        if(e<0)e+=mod;
                        *l+=l[m];
                        if(*l>=mod)*l-=mod;
                        l[m] = reduce(e* *it);
                    }
                }
            }
        }
        static void ifft(vector<int> &a){
            int n = a.size();
            for(int m=1;m<n;m<<=1){
                auto it_start = iomegas.begin()+m;
                auto it_end = it_start+m;
                for(auto l = a.begin();l!=a.end();l+=m){
                    for(auto it = it_start;it!=it_end;++it,++l){
                        l[m] = reduce(l[m]* *it);
                        int e = *l-l[m];
                        if(e<0)e+=mod;
                        *l+=l[m];
                        if(*l>=mod)*l-=mod;
                        l[m] = e;
                    }
                }
            }
        }
        static vector<int> mul(vector<int> a,vector<int> b){
            int need = a.size()+b.size()-1;
            int nbase = 1<<(32-__builtin_clz(need-1));
            a.resize(nbase);
            b.resize(nbase);
            fft(a);
            fft(b);
            for(int i=0;i<nbase;++i)a[i]=reduce(a[i]*1ll*b[i]);
            ifft(a);
            __uint64_t f = ((((1ll<<32)/nbase)%mod)*1ll*omegas[0])%mod;
            a.resize(need);
            for(int i=0;i<need;++i)a[i] = reduce(a[i]*f);
            return a;
        }
        static vector<int> inv(vector<int> &a){
            int n = a.size();
            vector<int> res(1,powmod<mod>(a[0]));
            int k = 1;
            while(k<n){
                int l = k<<1;
                int need = l<<1;
                if(l>n)a.resize(l);
                res.resize(need);
                vector<int> temp(a.begin(),a.begin()+l);
                temp.resize(need);
                fft(res);
                fft(temp);
                for(int i=0;i<need;++i)res[i] = reduce(temp[i]*1ll*reduce(res[i]*1ll*res[i]));
                ifft(res);
                __uint64_t f = ((((1ll<<32)/need)%mod)*1ll*omegas[0])%mod;
                f = (f<<32)%mod;
                res.resize(l);
                for(int i=0;i<l;++i)res[i]=reduce(f*res[i]);
                for(int i=k;i<l;++i)if(res[i])res[i]=mod-res[i];
                k = l;
            }
            a.resize(n);
            res.resize(n);
            return res;
        }
};

template<int max_base,int mod,int omega> const vector<__uint64_t> Ntt<max_base,mod,omega>::omegas = init_omegas();
template<int max_base,int mod,int omega> const vector<__uint64_t> Ntt<max_base,mod,omega>::iomegas = init_iomegas();

int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m;
    cin>>n>>m;
    vector<int> a(n),b(m);
    cin>>a>>b;
    a=Ntt<1<<20,998244353,3>::mul(a,b);
    cout<<a;
	return 0;
}
