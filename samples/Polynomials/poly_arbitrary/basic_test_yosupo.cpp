// Yosupo Problem Link : https://judge.yosupo.jp/problem/convolution_mod_1000000007
// Submission Link :https://judge.yosupo.jp/submission/227510

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
#include "../../../codes/Polynomials/poly_arbitrary.h"
#define vi  vector<int>
int main()
{
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    poly::init();
    int n,m;
    cin>>n>>m;
    vi a(n),b(m);
    cin>>a>>b;
    auto c = poly::mul(a,b);
    cout<<c;
	return 0;
}