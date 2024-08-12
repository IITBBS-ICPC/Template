// Yosupo Link : https://judge.yosupo.jp/problem/directedmst
// Submission Link : https://judge.yosupo.jp/submission/227528

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
#define vi  vector<int>

struct E { int s, t; ll w; }; // 0-base
struct PQ {
  struct P {
    ll v; int i;
    bool operator>(const P &b) const { return v > b.v; }
  };
  priority_queue<P, vector<P>, greater<>> pq; ll tag; // min heap
  void push(P p) { p.v -= tag; pq.emplace(p); }
  P top() { P p = pq.top(); p.v += tag; return p; }
  void join(PQ &b) {
    if (pq.size() < b.pq.size())
      swap(pq, b.pq), swap(tag, b.tag);
    while (!b.pq.empty()) push(b.top()), b.pq.pop();
  }
};
vector<int> dmst(const vector<E> &e, int n, int root) {
  vector<PQ> h(n * 2);
  for (int i = 0; i < int(e.size()); ++i)
    h[e[i].t].push({e[i].w, i});
  vector<int> a(n * 2); iota(all(a), 0);
  vector<int> v(n * 2, -1), pa(n * 2, -1), r(n * 2);
  auto o = [&](auto Y, int x) -> int {
    return x==a[x] ? x : a[x] = Y(Y, a[x]); };
  auto S = [&](int i) { return o(o, e[i].s); };
  int pc = v[root] = n;
  for (int i = 0; i < n; ++i) if (v[i] == -1)
    for (int p = i; v[p]<0 || v[p]==i; p = S(r[p])) {
      if (v[p] == i)
        for (int q = pc++; p != q; p = S(r[p])) {
          h[p].tag -= h[p].top().v; h[q].join(h[p]);
          pa[p] = a[p] = q;
        }
      while (S(h[p].top().i) == p) h[p].pq.pop();
      v[p] = i; r[p] = h[p].top().i;
    }
  vector<int> ans;
  for (int i = pc - 1; i >= 0; i--) if (v[i] != n) {
    for (int f = e[r[i]].t; f!=-1 && v[f]!=n; f = pa[f])
      v[f] = n;
    ans.push_back(r[i]);
  }
  return ans; // default minimize, returns edgeid array
}
// return ids of edges in mdst

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