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

template<int p>
class complex{
    private:
        int x,y;
    public:
        int base;
        complex(int base,int x=0,int y=0):x(x),y(y){}
        complex& operator*=(complex &other){
            int temp = helper<p>::normalise(other.y*1ll*y);
            y = helper<p>::normalise((other.y*1ll*x)+(other.x*1ll*y));
            x = helper<p>::normalise((other.x*1ll*x)+(base*1ll*temp));
        }
        static complex pow(complex a,int po){
            complex res(a.base,1);
            while(po>>1){
                if(po&1)res*=a;
                po>>=1;
                a*=a;
            }
            return res;
        }
};

template<int p1,int p2,int p3>
class fftnode{
    private:
        int a1,a2,a3;
        static __int128 const c1,c2,c3,c4;
    public:
        fftnode(ll x=0):a1(helper<p1>::normalise(x)),a2(helper<p2>::normalise(x)),a3(helper<p3>::normalise(x)){}
        void multiply(const fftnode& f){
            a1 = helper<p1>::normalise(a1*1ll*f.a1);
            a2 = helper<p2>::normalise(a2*1ll*f.a2);
            a3 = helper<p3>::normalise(a3*1ll*f.a3);
        }
        void sub(const fftnode&f){
            a1 = helper<p1>::normalise(f.a1+p1-helper<p1>::normalise(a1<<1));
            a2 = helper<p2>::normalise(f.a2+p2-helper<p2>::normalise(a2<<1));
            a3 = helper<p3>::normalise(f.a3+p3-helper<p3>::normalise(a3<<1));
        }
        void add(const fftnode&f){
            a1 = helper<p1>::normalise(f.a1+a1);
            a2 = helper<p2>::normalise(f.a2+a2);
            a3 = helper<p3>::normalise(f.a3+a3);
        }
        void mulsub(const fftnode&f){
            a1 = helper<p1>::normalise(a1*1ll*helper<p1>::normalise(2+p1-helper<p1>::normalise(f.a1*1ll*a1)));
            a2 = helper<p2>::normalise(a2*1ll*helper<p2>::normalise(2+p2-helper<p2>::normalise(f.a2*1ll*a2)));
            a3 = helper<p3>::normalise(a3*1ll*helper<p3>::normalise(2+p3-helper<p3>::normalise(f.a3*1ll*a3)));
        }
        void sqrt(){
            a1 = helper<p1>::cipollasqrt(a1);
            a2 = helper<p2>::cipollasqrt(a2);
            a3 = helper<p3>::cipollasqrt(a3);
        }
        void inverse(){
            a1 = helper<p1>::pow(a1);
            a2 = helper<p2>::pow(a2);
            a3 = helper<p3>::pow(a3);
        }
        int get(){
            return (((c1*a1)+(c2*a2)+(c3*a3))%c4)%mod;
        }
};

template<int p1,int p2,int p3> __int128 const fftnode<p1,p2,p3>::c4 = ((__int128)(p1*1ll*p2))*p3;
template<int p1,int p2,int p3> __int128 const fftnode<p1,p2,p3>::c1 = ((__int128)(p2*1ll*p3))*helper<p1>::pow(helper<p1>::normalise(p2*1ll*p3));
template<int p1,int p2,int p3> __int128 const fftnode<p1,p2,p3>::c2 = ((__int128)(p1*1ll*p3))*helper<p2>::pow(helper<p2>::normalise(p1*1ll*p3));
template<int p1,int p2,int p3> __int128 const fftnode<p1,p2,p3>::c3 = ((__int128)(p2*1ll*p1))*helper<p3>::pow(helper<p3>::normalise(p2*1ll*p1));

template<int p1>
class fftnodeS{
    private:
        int a1;
    public:
        fftnodeS(ll x=0):a1(helper<p1>::normalise(x)){}
        void multiply(const fftnodeS& f){
            a1 = helper<p1>::normalise(a1*1ll*f.a1);
        }
        void sub(const fftnodeS&f){
            a1 = helper<p1>::normalise(f.a1+p1-helper<p1>::normalise(a1<<1));
        }
        void add(const fftnodeS&f){
            a1 = helper<p1>::normalise(f.a1+a1);
        }
        void mulsub(const fftnodeS&f){
            a1 = helper<p1>::normalise(a1*1ll*helper<p1>::normalise(2+p1-helper<p1>::normalise(f.a1*1ll*a1)));
        }
        void sqrt(){
            a1 = helper<p1>::cipollasqrt(a1);
        }
        void inverse(){
            a1 = helper<p1>::pow(a1);
        }
        int get(){
            return helper<mod>::normalise(a1);
        }
};

template<typename T>
class Ntt {
    private:
        static int base;
        static T omega;
        static vector<T> omegas;
    public:
        static vector<int> rev,inv,fac,facinv;
        static void nadjust(int n){
            if(n<=fac.size()){
                for(int i=fac.size();i<=n;++i){
                    fac.pb(helper<mod>::normalise(fac.back()*1ll*i));
                    inv.pb(mod-helper<mod>::normalise((mod/i)*1ll*inv[mod%i]));
                    facinv.pb(helper<mod>::normalise(inv.back()*1ll*facinv.back()));
                }
            }

        }
        static void adjust(int nbase){
            if(nbase>base){
                while(omegas.size()<(1<<nbase))omegas.pb(T());
                rev.resize(1<<nbase);
            }
            while(base<nbase){
                omega.sqrt();
                int k =1<<base;
                for(int i=(1<<base)-1;i>=0;--i){
                    rev[i]<<=1;
                    rev[i+k] = rev[i]|1;
                    omegas[i<<1] = omegas[(i<<1)|1] = omegas[i];
                    omegas[(i<<1)|1].multiply(omega);
                }
                ++base;
            }
        }
        static void fft(vector<T> &a){
            int n = a.size();
            int nbase = __builtin_ctz(n);
            adjust(nbase);
            int shift = base-nbase;
            for(int i=0;i<n;++i){
                if(i<(rev[i]>>shift))swap(a[i],a[rev[i]>>shift]);
            }
            int r = 1<<base;
            for(int k=2;k<=n;k<<=1){
                r >>= 1;
                int l = k>>1;
                for(int j=0;j<n;j+=k){
                    for(int u = 0,s=0;u<l;++u,s+=r){
                        a[u+l+j].multiply(omegas[s]);
                        a[u+j].add(a[u+l+j]);
                        a[u+l+j].sub(a[u+j]);
                    }
                }
            }
        }
        static vector<T> mul(vector<T> A,vector<T> B){
            int need = A.size()+B.size()-1;
            int nbase = 1;
            while(nbase<need)nbase<<=1;
            while(A.size()<nbase)A.pb(T());
            while(B.size()<nbase)B.pb(T());
            fft(A);
            fft(B);
            for(int i=0;i<nbase;++i)A[i].multiply(B[i]);
            reverse(A.begin()+1,A.end());
            fft(A);
            A.resize(need);
            T inv(nbase);inv.inverse();
            for(auto &x:A)x.multiply(inv);
            return A;
        }
        static vector<T> inverse(vector<T> A) {
            int n = A.size();
            if(n==1){
                A[0].inverse();
                return A;
            }
            int m = (n+1)>>1;
            vector<T> B = inverse(vector<T> (A.begin(),A.begin()+m));
            int need = n<<1;
            int nbase = 1;
            while(nbase<need)nbase<<=1;
            while(A.size()<nbase)A.pb(T());
            while(B.size()<nbase)B.pb(T());
            fft(A);
            fft(B);
            for(int i=0;i<nbase;++i)B[i].mulsub(A[i]);
            T inv(nbase);inv.inverse();
            for(auto &x:B)x.multiply(inv);
            reverse(B.begin()+1,B.end());
            fft(B);
            B.resize(n);
            return B;
        }
        static vector<int> inverse(vector<int> a){
            int n = a.size();
            if(n==1){
                int x = helper<mod>::pow(a[0]);
                return {x};
            }
            int m = (n+1)>>1;
            vector<int> b = inverse(vector<int> (a.begin(),a.begin()+m));
            vector<T> B = convert(b);
            vector<T> G = mul(convert(a), B);
            vector<int> g = get(G);
            for(auto &x:g) x = helper<mod>::normalise(mod-x);
            g[0] = helper<mod>::normalise(2+g[0]);
            G = convert(g);
            B = mul(G,B);
            B.resize(n);
            return get(B);
        }
        static vector<int> integrate(vector<int> a){
            a.pb(0);
            nadjust(a.size());
            for(int i=1;i<a.size();++i)a[i]=helper<mod>::normalise(inv[i]*1ll*a[i-1]);
            a[0] = 0;
            return a;
        }
        static vector<int> derivative(vector<int> a){
            for(int i=1;i<a.size();++i)a[i-1] = helper<mod>::normalise(i*1ll*a[i]);
            a.pop_back();
            return a;
        }
        static vector<int> get(vector<T> A){
            vector<int> res(A.size());
            for(int i=0;i<res.size();++i)res[i]=A[i].get();
            return res;
        }
        static vector<T> convert(vector<int> a){
            vector<T> A;
            for(auto &x:a)A.pb(T(x));
            return A;
        }
};

template<typename T> int Ntt<T>::base;
template<typename T> T Ntt<T>::omega(1);
template<typename T> vector<T> Ntt<T>::omegas(1,Ntt<T>::omega);
template<typename T> vector<int> Ntt<T>::rev(1);
template<typename T> vector<int> Ntt<T>::inv(2,1);
template<typename T> vector<int> Ntt<T>::fac(2,1);
template<typename T> vector<int> Ntt<T>::facinv(2,1);
using Fnode = fftnode<P1,P2,P3>;

//assumption number is between [0,mod)
//use 3 mod , 2 mod according to range
//change sinverse to inverse in case of function friendly modulo

vector<int> mul(vector<int> a,vector<int> b){
    return Ntt<Fnode>::get(Ntt<Fnode>::mul(Ntt<Fnode>::convert(a),Ntt<Fnode>::convert(b)));
}
vector<int> inverse(vector<int> a){
    return Ntt<Fnode>::inverse(a);
}
int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int M = 2e5;
    vector<int> a(M,1),b(M,1);
    b=inverse(b);
    cout<<b[0]<<"\n";
    //a= mul(a,b);
    //cout<<a[0];
	return 0;
}
