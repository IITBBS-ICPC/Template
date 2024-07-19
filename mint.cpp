#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")
#include<bits/stdc++.h>
#define ll           long long
#define pb           push_back
#define F            first
#define S            second
#define mk           make_pair
#define pii          pair<int,int>
#define pll          pair<ll,ll>
#define vi           vector<int>
#define vll          vector<ll>
#define vN           vector<node>
#define vT           vector<T>
#define vpii         vector<pii>
#define vpll         vector<pll>
#define u32          uint32_t
#define u64          uint64_t
using namespace std;
template<typename T>istream & operator >> (istream &in, vT &a){for(auto &x:a)in>>x;return in;}
template<typename T>ostream & operator << (ostream &out, vT &a){for(auto &x:a)out<<x<<" ";return out;}
template<typename T,typename K>istream & operator >> (istream &in,  pair<T,K> &a){in>>a.F>>a.S;return in;}
template<typename T,typename K>ostream & operator << (ostream &out, pair<T,K> &a){out<<a.F<<" "<<a.S;return out;}

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
ll powmod(int a,int p,int mod){
    int res = 1;
    while(p){
        if(p&1)res=(res*1ll*a)%mod;
        p>>=1;
        a=(a*1ll*a)%mod;
    }
    return res;
}
vi pa,ipa;
const int N = 2e5;
template<int mod>
class Montgomery{
private:
        u32 val;
public:
    constexpr static u32 find2inv(){
        u32 inv = mod;
        for(int i=0;i<4;++i)inv *=(2-inv*mod);
        return -inv;
    }
    static const u32 N2 = find2inv();
    static const u64 modR = mod;
    constexpr static const u32 Rmod = (1ull<<32)%mod;
    constexpr static const u64 R2mod = (Rmod*1ull*Rmod) %mod;
    constexpr static const u64 R3mod = (R2mod*1ull*Rmod) %mod;
    static u32 mul(u64 t){t = (t+(modR*(u32(t)*N2)))>>32;if(t>=mod)t-=mod;return t;}
    static u32 modinverse(int x){
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
    static const u32 normalise(int v){
        if(v<0){
            if(v<-mod)v%=mod;
            v+=mod;
        }
        else if(v>=mod)v%=mod;
        return v;
    }
    Montgomery():val(0){}
    Montgomery(int v):val(mul(normalise(v)*R2mod)){}
    Montgomery(u32 val):val(val){}
    Montgomery& operator += (const Montgomery& other){val+=other.val;if(val>=mod)val-=mod;return *this;}
    Montgomery& operator -= (const Montgomery& other){val+=(mod-other.val);if(val>=mod)val-=mod;return *this;}
    Montgomery& operator *= (const Montgomery& other){
        val = mul(val*u64(other.val));
        return *this;
    }
    Montgomery operator + (const Montgomery& other){
        u32 temp = val+other.val;
        if(temp>=mod)temp-=mod;
        return Montgomery<mod> (temp);
    }
    Montgomery operator - (const Montgomery& other){
        u32 temp = val-other.val+mod;
        if(temp>=mod)temp-=mod;
        return Montgomery<mod> (temp);
    }
    Montgomery operator * (const Montgomery& other){
        return Montgomery<mod> (mul(val*u64(other.val)));;
    }
    Montgomery& operator /= (const Montgomery& other){
        u64 inv = modinverse(other.val);
        val = mul(R3mod*mul(val*inv));
        return *this;
    }
    Montgomery operator / (const Montgomery& other){
        u64 inv = modinverse(other.val);
        return Montgomery<mod> (mul(R3mod*mul(val*inv)));
    }
    Montgomery operator - (){
        return Montgomery()-*this;
    }
    Montgomery operator + (){
        return *this;
    }
    int get(){return mul(val);}
 	bool operator<(const Montgomery&other)const{return val<other.val;}
    template <int m>
    friend bool operator==(const Montgomery<m>& lhs, const int& rhs);
    template <int m>
    friend bool operator==(const Montgomery<m>& lhs, const Montgomery<m>& rhs);

    template <int m>
    friend std::istream& operator>>(std::istream& stream, Montgomery<m>& number);
    template <int m>
    friend std::ostream& operator<<(std::ostream& stream, Montgomery<m>& number);
};

template<int mod> bool operator == (const Montgomery<mod>& lhs, const int& rhs){return lhs.get()==rhs;}
template<int mod> bool operator == (const Montgomery<mod>& lhs, const Montgomery<mod>& rhs){return lhs.val==rhs.val;}
template<int mod> bool operator == (const int&rhs,const Montgomery<mod> &lhs){return lhs==rhs;}
template<int mod> Montgomery<mod> operator + (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)+=a;}
template<int mod> Montgomery<mod> operator - (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)-=a;}
template<int mod> Montgomery<mod> operator * (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)*=a;}
template<int mod> Montgomery<mod> operator / (const int&b,const Montgomery<mod> &a){return Montgomery<mod>(b)/=a;}
template<typename T> T pow(T a,int p){T res(1);while(p){if(p&1)res*=a;p>>=1;a*=a;};return res;}
const int mod = 1e9+7;
using mint = Montgomery<mod>;
struct Hnode{
	mint val;
	Hnode(int s=-1){
		val = (mint)((s>=0)?s:(int)rng());
	}
	void inverse(){val=1/val;}
	Hnode operator - ()const{
        Hnode temp(0);
        temp.val-=val;
        return temp;
    }
	Hnode& operator *= (const Hnode& other){
		val*=other.val;
		return *this;
	}
	Hnode& operator += (const Hnode& other){
		val+=other.val;
		return *this;
	}
	bool operator <(const Hnode &other)const{
		return val<other.val;
	}
    bool operator != (const Hnode &other)const{
        return !(val==other.val);
    }
};
template<typename T1,typename T2>
struct Gnode{
	T1 a;
	T2 b;
	Gnode(int s=-1):a(s),b(s){}
	void inverse(){
		a.inverse();
		b.inverse();
	}
	Gnode operator - ()const{
        Gnode temp(0);
        temp.a=-a;
		temp.b=-b;
        return temp;
    }
	Gnode& operator *= (const Gnode& other){
		a*=other.a;
		b*=other.b;
		return *this;
	}
	Gnode& operator += (const Gnode& other){
		a+=other.a;
		b+=other.b;
		return *this;
	}
	bool operator <(const Gnode &other)const{
		if(a!=other.a)return a<other.a;
		return b<other.b;
	}
    bool operator != (const Gnode &other)const{
        return (a!=other.a)||(b!=other.b);
    }
};
using Node = Gnode<Hnode,Hnode>;
class node{
    public:
    Node val;
    static vector<Node> P,iP;
    const static Node A;
    node(int s=-1):val(s){}
    static void pre(int N){
        P.pb(Node(1));
		iP.pb(Node(1));
		Node iA = A;
		iA.inverse();
		for(int i=1;i<N;++i){
			P.pb(P[i-1]);
			P[i]*=A;
			iP.pb(iP[i-1]);
			iP[i]*=iA;
		}
    }
    node operator - ()const{
        node temp(0);
        temp.val=-val;
        return temp;
    }
    node& operator += (const node &other) {
        val+=other.val;
        return *this;
    }
    void div(int s){
        val *= iP[s];
    }
    void mul(int s){
        val *= P[s];
    }
	bool operator <(const node &other)const{
		return val<other.val;
	}
    bool operator != (const node &other)const{
        return (val!=other.val);
    }
};
const Node node::A = Node();
vector<Node> node::P = {};
vector<Node> node::iP = {};
template<typename T>
class Fen{
    vT fen;
    public:
        Fen(int n,T z = 0){
            fen.resize(n+1,z);
        }
        void update(int i,T val){
            while(i<fen.size()){
                fen[i]+=val;
                i+=(i&(-i));
            }
        }
        T get(int i,T z = 0){
            T ans = z;
            while(i){
                ans+=fen[i];
                i-=(i&(-i));
            }
            return ans;
        }
};
