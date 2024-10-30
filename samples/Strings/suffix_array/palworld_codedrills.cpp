// CodeDrills Problem Palworld : https://codedrills.io/contests/icpc-amritapuri-2023-regional-round/problems/palworld
// Submission Link : https://codedrills.io/submissions/972473

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include<bits/stdc++.h>
#define pii pair<int,int>
#define F first
#define S second
using namespace std;
// Time Complexity: O(nlogn)

void count_sort(vector<int> &p,vector<int> &c){  // Sorts values in p by keys in c i.e, if c[p[i]] < c[p[j]] then i appears before j in p.
    int n = p.size();
    vector<int> cnt(n);
    for(auto x : c) cnt[x]++;
    vector<int> p_new(n),pos(n);
    pos[0] = 0;
    for(int i=1;i<n;++i) pos[i] = pos[i-1] + cnt[i-1];
    for(auto x : p){
        int i = c[x];
        p_new[pos[i]] = x;
        pos[i]++;
    }
    p = p_new;
}
vector<int> lcp_array(vector<int> &p,vector<int> &c,string s){
    int n = p.size();
    vector<int> lcp(n-1);
    int k=0;
    for(int i=0;i<n-1;++i){
        int pi = c[i];
        int j = p[pi-1];
        //lcp[i] = lcp[s[i...],s[j...]]
        while(s[i+k] == s[j+k]) ++k;
        lcp[pi-1] = k;  
        k = max(k-1,0);
    }
    return lcp;
}

// Returns sorted suffix_array of size (n+1) with first element = n (empty suffix).
pair<vector<int>,vector<int>> suffix_array(string s){
    s += '$';
    int n = s.size();
    vector<int> p(n),c(n); // p stores suffix array and c stroes its equivalence class
    {
        // k = 0 phase
        vector<pair<char,int>> a(n);
        for(int i=0;i<n;++i) a[i] = {s[i],i};
        sort(a.begin(),a.end());
        for(int i=0;i<n;++i) p[i] = a[i].S;
        c[p[0]] = 0;
        for(int i=1;i<n;++i){
            if(a[i].F == a[i-1].F) c[p[i]] = c[p[i-1]];
            else c[p[i]] = c[p[i-1]] + 1;
        }
    }
    int k=0;
    while((1<<k) < n ){ // k -> k + 1
        // We require to sort p by {c[i], c[i+(1<<k)]} value. So we use radix sort: Sort the second element of pair then count sort first element in a stable fashion.
        // Sort by second element: p[i] -= (1<<k). As p[i]'s are already sorted by c[i] values.
        for(int i = 0; i < n ; ++i) p[i] = (p[i] - (1<<k) + n )%n;
        count_sort(p,c);
        
        vector<int> c_new(n);
        c_new[p[0]] = 0;
        for(int i=1;i<n;++i){
            pii prev = {c[p[i-1]] , c[(p[i-1] + (1<<k))%n] } ;
            pii now = {c[p[i]] , c[(p[i] + (1<<k))%n] } ;
            if( now == prev) c_new[p[i]] = c_new[p[i-1]];
            else c_new[p[i]] = c_new[p[i-1]] + 1;
        }
        c = c_new;
        ++k;
    }
    return make_pair(p,lcp_array(p,c,s)); 
}

void solve(){
	int n,K;
	cin>>n>>K;
	int ans = min(n,K)+K;
	string s;
	cin>>s;
	string h = s+"#";
	reverse(s.begin(),s.end());
	h+=s;
	auto [suf,lcp] = suffix_array(h);
	vector<int> rev(h.length()+1);
	for(int i=0;i<suf.size();++i){
		rev[suf[i]] = i;
	}
	int E = suf.size();
	vector<int> lg(E);
	for(int i=2;i<E;++i)lg[i]=lg[i>>1]+1;
	int L = lg.back();
	vector<vector<int>> table(lcp.size(),vector<int>(L+1));
	for(int i=lcp.size()-1;i>=0;--i){
		table[i][0] = lcp[i];
		for(int j=1;(i+(1<<j))<=lcp.size();++j){
			table[i][j] = min(table[i][j-1],table[i+(1<<(j-1))][j-1]);
		}
	}
	reverse(s.begin(),s.end());
	int m = h.length()-1;
	function<int(int,int)> getPreSufMin = [&](int i,int j){
		i = rev[m-i],j = rev[j];
		if(i>j)swap(i,j);
		int l = j-i;
		int u = lg[l];
		int f = min(table[i][u],table[j-(1<<u)][u]);
		return f;
	};
	for(int i=0;i<n;++i){
		for(int j=i+1,s=0;(s<=(K+1))&&(j<n);++j,++s){
			ans=max(ans,(getPreSufMin(i,j)<<1)+s+K);
		}
		int cur = getPreSufMin(i,i);
		for(int k=0;k<=K;++k)
		{
			int l = i-cur;
			int r = i+cur+k;
			if((r<n)&&(l>=0)){
				ans = max(ans,((cur+k+getPreSufMin(l,r))<<1)-1);
			}
			else if(r<n){
				ans = max(ans,((cur+k)<<1)-1);
			}
			else{
				ans = max(ans,((n-i)<<1)-1);
			}
		}
		for(int k=0;k<=K;++k)
		{
			int l = i-cur-k;
			int r = i+cur;
			if((r<n)&&(l>=0)){
				ans = max(ans,((cur+k+getPreSufMin(l,r))<<1)-1);
			}
			else if(l>=0){
				ans = max(ans,((cur+k)<<1)-1);
			}
			else{
				ans = max(ans,(i<<1)+1);
			}
		}
	}
	for(int i=1;i<n;++i){
		int cur = getPreSufMin(i-1,i);
		for(int k=0;k<=K;++k)
		{
			int l = i-1-cur;
			int r = i+cur+k;
			if((r<n)&&(l>=0)){
				ans = max(ans,(cur+k+getPreSufMin(l,r))<<1);
			}
			else if(r<n){
				ans = max(ans,(cur+k)<<1);
			}
			else{
				ans = max(ans,(n-i)<<1);
			}
		}
		for(int k=0;k<=K;++k)
		{
			int l = i-1-cur-k;
			int r = i+cur;
			if((r<n)&&(l>=0)){
				ans = max(ans,(cur+k+getPreSufMin(l,r))<<1);
			}
			else if(l>=0){
				ans = max(ans,(cur+k)<<1);
			}
			else{
				ans = max(ans,(i<<1));
			}
		}
	}
	cout<<ans<<"\n";
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int t;
	cin>>t;
	while(t--)solve();
}