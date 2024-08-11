// https://judge.yosupo.jp/problem/directedmst

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
#define ll long long
#define all(x) x.begin(),x.end()
#include "../../../codes/Graphs/minimum_directed_spanning_tree.h"
#define vi  vector<int>
int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m,s;
    cin>>n>>m>>s;
    vector<E> edges(m);
    for(auto &e:edges){
        cin>>e.s>>e.t>>e.w;
    }
    auto ans = dmst(edges,n,s);
    ll total = 0;
    vector<int> par(n,-1);
    for(auto id:ans){
        auto [s,t,w] = edges[id];
        total+=w;
        par[t] = s;
    }
    par[s] = s;
    cout<<total<<"\n"<<par<<"\n";
	return 0;
}