/*
Implementation of Dinic's blocking algorithm
for the maximum flow.
Complexity: V^2 E (faster on real graphs).

please add edges not related to input first
to improve constants

This class accepts a graph
(costructed calling AddEdge) and then solves
the maximum flow problem for any source and sink.

Both directed and undirected graphs are supported.
In case of undirected graphs,
each edge must be added twice.

To compute the maximum flow just call
GetMaxFlowValue(source, sink).
*/

// Flow data type
#define pb push_back
using T = int;
struct Edge {
  int u, v;
  T cap, flow;
};
// Define object globally with limits on number of
// vertices an edges (N, M) as templates
template <int N, int M> struct Dinic {
  T inf = 1e9;
  int esz = 0, n, level[N], ptr[N];
  Edge edge[2 * M];
  vector<int> g[N];
  void init(int _n) { n = _n; }
  void addEdge(int u, int v, int cap) {
    edge[esz] = {u, v, cap, 0};
    edge[esz + 1] = {v, u, 0, 0};
    g[u].pb(esz);
    g[v].pb(esz + 1);
    esz += 2;
  }
  bool bfs(int s, int t) {
    memset(level, -1, N * 4);
    queue<int> q;
    q.push(s), level[s] = 0;
    while(q.size()) {
      int v = q.front();
      q.pop();
      for(int x : g[v]) {
        Edge &e = edge[x];
        if(e.cap > e.flow && level[e.v] == -1)
          level[e.v] = level[e.u] + 1, q.push(e.v);
      }
    }
    return level[t] != -1;
  }
  T dfs(int v, int t, T pf) {
    if(v == t || !pf) return pf;
    T f = 0;
    for(int &i = ptr[v]; i < g[v].size(); i++) {
      int ind = g[v][i];
      Edge &e = edge[ind], &re = edge[ind ^ 1];
      if(level[e.u] != level[e.v] - 1) continue;
      T tf = dfs(e.v, t, min(pf - f, e.cap - e.flow));
      f += tf, e.flow += tf, re.flow -= tf;
      if(f == pf) return f;
    }
    return f;
  }
  T calc(int s, int t) {
    T f = 0;
    while(bfs(s, t)) {
      memset(ptr, 0, N * 4);
      f += dfs(s, t, inf);
    }
    return f;
  }
};
