// problem link:
// https://codeforces.com/contest/1404/problem/E
// submission link:
// https://codeforces.com/contest/1404/submission/304772291

#include <bits/stdc++.h>

// #include <ext/pb_ds/paroc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>
// using namespace __gnu_pbds;

// #define ONLINE_JUDGE
#ifndef ONLINE_JUDGE

#include "./debug/debug.h"
#define debug(x)                                       \
  cerr << #x << " ";                                   \
  _print(x);                                           \
  cerr << endl;

#else
#define debug(x)
#endif

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
using namespace std;
// 1 1 2 3 1 2

using T = int;
struct Edge {
  int u, v;
  T cap, flow;
};
template <int N, int M> struct Dinic {
  T inf = 1e9;
  int esz = 0, n, lev[N], ptr[N];
  Edge es[2 * M];
  vector<int> adj[N];
  void init(int _n) { n = _n; }
  void addEdge(int u, int v, int cap) {
    es[esz] = {u, v, cap, 0};
    es[esz + 1] = {v, u, 0, 0};
    adj[u].push_back(esz);
    adj[v].push_back(esz + 1);
    esz += 2;
  }
  bool bfs(int s, int t) {
    memset(lev, -1, N * sizeof(int));
    queue<int> q;
    q.push(s), lev[s] = 0;
    while(q.size()) {
      int v = q.front();
      q.pop();
      for(int x : adj[v]) {
        Edge &e = es[x];
        if(e.cap > e.flow && lev[e.v] == -1)
          lev[e.v] = lev[e.u] + 1, q.push(e.v);
      }
    }
    return lev[t] != -1;
  }
  T dfs(int v, int t, T pf) {
    if(v == t || !pf) return pf;
    T f = 0;
    for(int &i = ptr[v]; i < adj[v].size(); i++) {
      int ind = adj[v][i];
      Edge &e = es[ind], &re = es[ind ^ 1];
      if(lev[e.u] != lev[e.v] - 1) continue;
      T tf = dfs(e.v, t, min(pf - f, e.cap - e.flow));
      f += tf, e.flow += tf, re.flow -= tf;
      if(f == pf) return f;
    }
    return f;
  }
  T calc(int s, int t) {
    T f = 0;
    while(bfs(s, t)) {
      memset(ptr, 0, N * sizeof(int));
      f += dfs(s, t, inf);
    }
    return f;
  }
};

Dinic<2 * 210 * 210, 8 * 210 * 210> g;
bool solve() {
  int n, m;
  cin >> n >> m;

  vector<string> s(n);
  for(auto &x : s)
    cin >> x;
  int nv = 2 * (n + 1) * (m + 1);
  g.init(nv);

  vector<vector<int>> h2vert(n + 5, vector<int>(m + 5));
  vector<vector<int>> v2vert(n + 5, vector<int>(m + 5));

  int k = 1;
  for(auto &x : h2vert)
    for(auto &y : x)
      y = k++;

  for(auto &x : v2vert)
    for(auto &y : x)
      y = k++;

  debug(h2vert);
  debug(v2vert);

  vector<pair<int, int>> edges;
  set<int> vert, hor;

  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      if(s[i][j] != '#') continue;
      int top = h2vert[i][j];
      int bottom = h2vert[i + 1][j];
      int left = v2vert[i][j];
      int right = v2vert[i][j + 1];

      if(i && s[i - 1][j] == '#') {
        if(j && s[i][j - 1] == '#') {
          edges.push_back({top, left});
        }
        if(j < m - 1 && s[i][j + 1] == '#') {
          edges.push_back({top, right});
        }
      }
      if(i < n - 1 && s[i + 1][j] == '#') {
        if(j && s[i][j - 1] == '#') {
          edges.push_back({bottom, left});
        }
        if(j < m - 1 && s[i][j + 1] == '#') {
          edges.push_back({bottom, right});
        }
      }
    }
  }
  debug(k);
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      if(i < n - 1 && s[i][j] == '#'
         && s[i + 1][j] == '#')
        vert.insert(h2vert[i + 1][j]);
      if(j < m - 1 && s[i][j] == '#'
         && s[i][j + 1] == '#')
        hor.insert(v2vert[i][j + 1]);
    }
  }

  for(auto x : hor) {
    g.addEdge(x, 0, 1);
  }

  debug(hor);
  debug(vert);

  for(auto x : vert) {
    g.addEdge(k + 1, x, 1);
  }

  for(auto e : edges) {
    auto [u, v] = e;
    debug(e);
    g.addEdge(u, v, 1);
  }

  int vertexCover = g.calc(k + 1, 0);
  debug(vertexCover);
  int independantSet
    = hor.size() + vert.size() - vertexCover;

  int ans = -independantSet;
  for(auto x : s) {
    for(auto y : x)
      if(y == '#') ans++;
  }
  cout << ans << '\n';

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