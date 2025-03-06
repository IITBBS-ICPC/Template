typedef vector<int> vi;
bool ckmin(int &a, int b) {
  return b < a ? a = b, true : false;
}
bool ckmin(ll &a, ll b) {
  return b < a ? a = b, true : false;
}

struct MCMF {
  using F = int;
  using C = ll; // flow type, cost type
  struct Edge {
    int to;
    F flo, cap;
    C cost;
  };
  int N;
  vector<C> pi, dis;
  vi prev;
  vector<Edge> edge;
  vector<vi> g;
  void init(int _N) {
    N = _N;
    pi.resize(N), dis.resize(N), prev.resize(N),
      g.resize(N);
  }
  void addEdge(int u, int v, F cap, C cost) {
    assert(cap >= 0);
    g[u].pb(edge.size());
    edge.pb({v, 0, cap, cost});
    g[v].pb(edge.size());
    edge.pb({u, 0, 0, -cost});
  } // use asserts, don't try smth dumb
  bool path(int s, int t) { // find lowest cost path to
                            // send flow through
    const C inf = 1e18;
    for(int i = 0; i < N; i++)
      dis[i] = inf;
    using T = pair<C, int>;
    priority_queue<T, vector<T>, greater<T>> pq;
    pq.push({dis[s] = 0, s});
    while(pq.size()) { // Dijkstra
      T x = pq.top();
      pq.pop();
      if(x.first > dis[x.second]) continue;
      for(int e : g[x.second]) {
        const Edge &E = edge[e]; // all weights should be
                                // non-negative
        if(E.flo < E.cap
           && ckmin(dis[E.to], x.first + E.cost
                                  + pi[x.second]
                                  - pi[E.to]))
          prev[E.to] = e, pq.push({dis[E.to], E.to});
      }
    } // if costs are doubles, add some EPS so you
    // don't traverse ~0-weight cycle repeatedly
    return dis[t] != inf; // return flow
  }
  void setpi() { // Call this function before calc if
                 // have -ve weights
    for(int i = 0; i < N; i++) {
      for(int e = 0; e < edge.size(); e++) {
        const Edge &E = edge[e]; // Bellman-Ford
        if(E.cap)
          ckmin(pi[E.to], pi[edge[e ^ 1].to] + E.cost);
      }
    }
  }
  pair<F, C> calc(int s, int t, int f) {
    assert(s != t);
    // setpi();
    F totFlow = 0;
    C totCost = 0;
    while(path(s, t)
          && f) { // p -> potentials for Dijkstra
      for(int i = 0; i < N; i++)
        pi[i] += dis[i]; // don't matter for
                          // unreachable nodes
      F df = f;
      for(int x = t; x != s; x = edge[prev[x] ^ 1].to) {
        const Edge &E = edge[prev[x]];
        ckmin(df, E.cap - E.flo);
      }
      f -= df;
      totFlow += df;
      totCost += (pi[t] - pi[s]) * df;
      for(int x = t; x != s; x = edge[prev[x] ^ 1].to)
        edge[prev[x]].flo += df, edge[prev[x] ^ 1].flo
                               -= df;
    } // get max flow you can send along path
    return {totFlow, totCost};
  }
};