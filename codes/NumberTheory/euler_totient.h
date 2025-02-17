const int N=2e5+1;
int phi[N];

void etf(){
	for(int i=0;i<N;i++) phi[i]=i;
	for(int i=2;i<N;i++) if(phi[i]==i) for(int j=i;j<N;j+=i) phi[j]-=phi[j]/i;
}
int phi(int n){
	// Finds in O(sqrt(N)) for a single n.
	int ans=n;
	for(int i=2;i*i<=n;i++){
		if(!(n%i)){
			ans-=ans/i;
			while(!(n%i)) n/=i;
		}
	}
	if(n>1) ans-=ans/n;
	return ans;
}

