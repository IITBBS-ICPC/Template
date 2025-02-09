// problem link: https://codeforces.com/contest/237/problem/E
// submission link: https://codeforces.com/contest/237/submission/305371591

#include <bits/stdc++.h>

// #include <ext/push_back_ds/paroc_container.hpp>
// #include <ext/push_back_ds/tree_policy.hpp>
// using namespace __gnu_push_backds;

using namespace std;

// #define ONLINE_JUDGE
#ifndef ONLINE_JUDGE

#include "./debug/debug.h"
#define debug(x)                                                              \
  cerr << #x << " ";                                                          \
  _print(x);                                                                  \
  cerr << endl;

#else
#define debug(x)
#endif
// test
#define ll long long
#define lld long double
#define vi vector<int>
#define vll vector<ll>
#define pii pair<int, int>
#define mi map<int, int>
#define sti set<int>
#define sp << ' ' <<
#define spl << ' '
#define nl << '\n'
#define pb push_back
#define all(x) x.begin(), x.end()
#define allr(x) x.rbegin(), x.rend()

#define sz(a) (int)a.size()

// 1 1 2 3 1 2

bool ckmin(int &a, int b) { return b < a ? a = b, true : false; }
bool ckmin(ll &a, ll b) { return b < a ? a = b, true : false; }

struct MCMF {
  using F = ll;
  using C = ll; // flow type, cost type
  struct Edge {
    int to;
    F flo, cap;
    C cost;
  };
  int N;
  vector<C> p, dist;
  vector<int> pre;
  vector<Edge> eds;
  vector<vector<int>> adj;
  void init(int _N) {
    N = _N;
    p.resize(N), dist.resize(N), pre.resize(N), adj.resize(N);
  }
  void addEdge(int u, int v, F cap, C cost) {
    assert(cap >= 0);
    adj[u].pb(eds.size());
    eds.pb({v, 0, cap, cost});
    adj[v].pb(eds.size());
    eds.pb({u, 0, 0, -cost});
  } // use asserts, don't try smth dumb
  bool path(int s, int t) { // find lowest cost path to send flow through
    const C inf = 1e18;
    for(int i = 0; i < N; i++)
      dist[i] = inf;
    using T = pair<C, int>;
    priority_queue<T, vector<T>, greater<T>> todo;
    todo.push({dist[s] = 0, s});
    while(todo.size()) { // Dijkstra
      T x = todo.top();
      todo.pop();
      if(x.first > dist[x.second])
        continue;
      for(int e : adj[x.second]) {
        const Edge &E = eds[e]; // all weights should be non-negative
        if(E.flo < E.cap
           && ckmin(dist[E.to], x.first + E.cost + p[x.second] - p[E.to]))
          pre[E.to] = e, todo.push({dist[E.to], E.to});
      }
    } // if costs are doubles, add some EPS so you
    // don't traverse ~0-weight cycle repeatedly
    return dist[t] != inf; // return flow
  }
  void setpi() { // Call this function before calc if have -ve weights
    for(int i = 0; i < N; i++) {
      for(int e = 0; e < eds.size(); e++) {
        const Edge &E = eds[e]; // Bellman-Ford
        if(E.cap)
          ckmin(p[E.to], p[eds[e ^ 1].to] + E.cost);
      }
    }
  }
  pair<F, C> calc(int s, int t, int f) {
    assert(s != t);
    // setpi();
    F totFlow = 0;
    C totCost = 0;
    while(path(s, t) && f) { // p -> potentials for Dijkstra
      for(int i = 0; i < N; i++)
        p[i] += dist[i]; // don't matter for unreachable nodes
      F df = f;
      for(int x = t; x != s; x = eds[pre[x] ^ 1].to) {
        const Edge &E = eds[pre[x]];
        ckmin(df, E.cap - E.flo);
      }
      f -= df;
      totFlow += df;
      totCost += (p[t] - p[s]) * df;
      for(int x = t; x != s; x = eds[pre[x] ^ 1].to)
        eds[pre[x]].flo += df, eds[pre[x] ^ 1].flo -= df;
    } // get max flow you can send along path
    return {totFlow, totCost};
  }
};
const int N = 110;

int k = 0;
map<pii, int> mp;

int get(int x, int y) {
  if(mp.count({x, y}))
    return mp[{x, y}];
  else
    return mp[{x, y}] = k++;
}

bool solve() {
  string t;
  cin >> t;

  int n;
  cin >> n;

  vector<array<int, 4>> edges;

  int source = get(-1, 0), sink = get(-1, 1);
  for(int i = 0; i < n; i++) {
    string s;
    cin >> s;
    int a;
    cin >> a;

    edges.push_back({source, get(i, -1), a, i + 1});

    map<char, int> freq;
    for(auto x : s)
      freq[x]++;

    for(auto [ch, f] : freq) {
      edges.push_back({get(i, -1), get(i, ch), f, 0});
      edges.push_back({get(i, ch), get(n, ch), (int)1e9, 0});
    }
  }

  map<char, int> freq;
  for(auto x : t)
    freq[x]++;

  for(auto [ch, f] : freq) {
    edges.push_back({get(n, ch), sink, f, 0});
  }

  MCMF g;
  g.init(k + 5);
  for(auto e : edges) {
    g.addEdge(e[0], e[1], e[2], e[3]);
  }

  auto [flow, cost] = g.calc(source, sink, t.size());
  if(flow == t.size())
    cout << cost << '\n';
  else
    cout << -1 << '\n';

  return false;
}

signed main() {
#ifndef ONLINE_JUDGE
  freopen("error.txt", "w", stderr);
#endif
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  srand(0);

  int tc = 1;
  // cin >> tc;
  while(tc--) {
    if(solve()) {
      // cout << "YES\n";
    } else {
      // cout << "NO\n";
    }
  }

  return 0;
}