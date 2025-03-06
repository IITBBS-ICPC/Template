typedef long long ll;
const int P1 = 880803841, G1 = 26;//(105*2^23)+1
const int P2 = 897581057, G2 = 3;//(107*2^23)+1
const int P3 = 998244353, G3 = 3;//(119*2^23)+1

typedef vector<int> vi;
#define pb push_back
#define f(n) for(int i=0;i<(n);i++)

const int M = 998244353; // Also works on - 880803841 and 897581057
int expo(int a,int b,int mod=M){int ans=1; while(b){if(b&1) ans=(ans*1ll*a)%mod; a=(a*1ll*a)%mod; b>>=1;} return ans;}
int mod_div(int a,int b,int mod=M){return (a*1ll*expo(b,mod-2))%mod;}

const int root=62; // For 998244353. NOT SURE-> Otherwise while(expo(root,M/2)==1) root++;
void ntt(vi &a){
	int n=a.size(),L=31-__builtin_clz(n);
	static vi rt(2,1);
	
	for(static int k=2,s=2;k<n;k*=2,s++) {
		rt.resize(n);
		int z[]={1,expo(root,M>>s)};
		for(int i=k;i<2*k;i++) rt[i]=(ll)rt[i/2]*z[i&1]%M;
	}
	vi rev(n);
	f(n){
		rev[i]=(rev[i/2]|(i&1)<<L)/2;
		if(i<rev[i]) swap(a[i],a[rev[i]]);
	}
	for(int k=1;k<n;k*=2)
		for(int i=0;i<n;i+=2*k)
			for(int j=0;j<k;j++){
				int z=(ll)rt[j+k]*a[i+j+k]%M,&ai=a[i+j];
				a[i+j+k]=ai-z+(z>ai?M:0);
				ai+=(ai+z>=M?z-M:z);
			}
}
bool cap=true; // Set to FALSE to get n+m-1 size product. Set to TRUE for Exp() and below Operations
#define ao(a)                                          \
  for(int _i = 0; _i < a.size(); _i++)                 \
    cout << ((2 * a[_i] < M) ? a[_i] : a[_i] - M)      \
         << " ";
vi brutemul(const vi &a,const vi &b){
	int n=a.size(),m=b.size(),s=n+m-(cap? min(n,m):1);
	vi out(s,0);
	f(n) for(int j=0;j<min(m,s-i);j++) out[i+j]=(out[i+j]+(a[i]*1ll*b[j]%M))%M;
	return out;
}
vi mul(const vi &a,const vi &b){
	if(a.empty() || b.empty()) return {};
	int p=a.size(),q=b.size();
	if(min(p,q)<32) return brutemul(a,b);
	int s=p+q-(cap? min(p,q):1),n=1<<(32-__builtin_clz(s));
	int inv=mod_div(1,n);
    vi L(a),R(b),out(n);
	L.resize(n); R.resize(n);
	ntt(L); ntt(R);
	f(n) out[-i&(n-1)]=(ll)L[i]*R[i]%M*inv%M;
	ntt(out);
	return {out.begin(),out.begin()+s};
}

#define opr operator
vi& opr*=(vi& a,const vi &b){return a=mul(a,b);}
vi opr*(const vi& a,const vi &b){return mul(a,b);}
vi opr*(const int k,const vi &P){vi Q(P); for(int &a:Q) a=(a*1ll*k)%M; return Q;}
vi& opr*=(vi& P,const int k){for(int &a:P) a=(a*1ll*k)%M; return P;}
vi& opr+=(vi& a,const vi &b){
    if(a.size()<b.size()) a.resize(b.size());
    for(int i=0;i<b.size();++i){
        a[i]+=b[i];
        if(a[i]>=M) a[i]-=M;
    }
    return a;
}
vi opr+(const vi& a,const vi &b){vi c(a); return c+=b;}
vi& opr-=(vi& a,const vi &b){
    if(a.size()<b.size()) a.resize(b.size());
    for(int i=0;i<b.size();++i){
        a[i]-=b[i];
        if(a[i]<0) a[i]+=M;
    }
    return a;
}
vi opr-(const vi& a,const vi &b){vi c(a); return c-=b;}
vi opr/(const int k,const vi &P){ // P[0] should be non-zero
	vi Q{mod_div(1,P[0])};
	int n=P.size();
	for(int k=2;Q.size()<n;k*=2){
		Q*=(vi{2}-Q*vi(P.begin(),P.begin()+k));
		Q.resize(k);	
	}
	Q*=k;
	return Q;
}
vi& opr/=(vi& a,const vi &b){return a*=1/b;}
vi opr/(const vi& a,const vi &b){vi c(a); return c/=b;}
int degree(const vi &a) {
  int n = a.size();
  while(n && !a[n-1]) n--;
  return n;
}
vi& opr%=(vi &a,const vi &b){
	if(a.empty()) return a;
	int deg=degree(a)-degree(b)+1;
	if(deg<=0) return a;
	vi X(b.rbegin(),b.rend()); X.resize(deg);
	vi Y(a.rbegin(),a.rend()); Y.resize(deg);	
	if(deg&(deg-1)) X.resize(1<<(__lg(deg)+1));
	vi Q=Y/X; Q.resize(deg);
	reverse(Q.begin(),Q.end());
	a-=Q*b; a.pop_back();
	while(a.size() && !a.back()) a.pop_back();
	return a;
}
vi opr%(const vi &a,const vi &b){vi v=a; return v%=b;}

// All P[i] should be in range [0,M)
// P.size() should be a power of 2.
void deriv(vi &P){
    f(P.size()-1) P[i]=(P[i+1]*1ll*(i+1))%M;
    P.pop_back();
}

const int N=1e5+1,N1=1<<(__lg(N)+1);
int inv_mod[N1+1]; // Only required for below Poly functions
void IM(){f(N1) inv_mod[i+1]=(mod_div(1,i+1));}

void Integrate(vi &P,int C){ // C is integration constant =: P[0]
	P.pb(0);
    for(int i=P.size()-1;i>=0;i--) P[i+1]=(P[i]*1ll*inv_mod[i+1])%M;
    P[0]=C%M;
}
void Log(vi &P){ // P[0] should be 1
	vi invP=1/P;
    deriv(P);
    P*=invP;
    Integrate(P,0);
    P.resize(invP.size());
}
// NOTE:- Set cap=true, for functions below or use Q.resize(k) at the end of loop of Exp().
void Exp(vi &P){ // P[0] should be 0
	int n=P.size();
	vi Q={1};
	for(int k=2;Q.size()<n;k*=2){
		vi lnQ(Q);
		lnQ.resize(k); 
		Log(lnQ);
		Q*=(vi(P.begin(),P.begin()+k)+vi{1}-lnQ); 
	}
	swap(P,Q);
}
void Power(vi &P,int k){
    int n=P.size(),C=P[0],shift=0;
    if(!C){
        while(shift<n && !P[shift]) shift++;
        if(shift*k>=n){
        	memset(&P[0],0,n*sizeof(P[0]));
			return;
		}
		P=vi(P.begin()+shift,P.end()-shift*(k-1));
        C=P[0];
    }
    if(C!=1){
        P*=mod_div(1,C);
        C=expo(C,k);
    }
    Log(P); 
	P*=k;
    Exp(P); 
    if(C!=1) for(int &a:P) a=(a*1ll*C)%M;
    if(shift){
    	vi Q(n,0);
    	f(P.size()) Q[i+shift*k]=P[i];
    	swap(P,Q);
    }
}
void Root(vi &P,int k){ // P[0] should be 1
	Log(P);
	P*=mod_div(1,k);
	Exp(P);
}
