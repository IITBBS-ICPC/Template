bool ckmin(int &a, int b) {
  return b < a ? a = b, true : false;
}
bool ckmin(ll &a, ll b) {
  return b < a ? a = b, true : false;
}

struct MCMF {
  using F = ll;
  using C = ll; // flow type, cost type
  struct Edge {
    int to;
    F flo, cap;
    C cost;
  };
  int N;
  vector<C> pi, dist;
  vector<int> pre;
  vector<Edge> eds;
  vector<vector<int>> adj;
  void init(int _N) {
    N = _N;
    pi.resize(N), dist.resize(N), pre.resize(N),
      adj.resize(N);
  }
  void addEdge(int u, int v, F cap, C cost) {
    assert(cap >= 0);
    adj[u].pb(eds.size());
    eds.pb({v, 0, cap, cost});
    adj[v].pb(eds.size());
    eds.pb({u, 0, 0, -cost});
  } // use asserts, don't try smth dumb
  bool path(int s, int t) { // find lowest cost path to
                            // send flow through
    const C inf = 1e18;
    for(int i = 0; i < N; i++)
      dist[i] = inf;
    using T = pair<C, int>;
    priority_queue<T, vector<T>, greater<T>> todo;
    todo.push({dist[s] = 0, s});
    while(todo.size()) { // Dijkstra
      T x = todo.top();
      todo.pop();
      if(x.first > dist[x.second]) continue;
      for(int e : adj[x.second]) {
        const Edge &E = eds[e]; // all weights should be
                                // non-negative
        if(E.flo < E.cap
           && ckmin(dist[E.to], x.first + E.cost
                                  + pi[x.second]
                                  - pi[E.to]))
          pre[E.to] = e, todo.push({dist[E.to], E.to});
      }
    } // if costs are doubles, add some EPS so you
    // don't traverse ~0-weight cycle repeatedly
    return dist[t] != inf; // return flow
  }
  void setpi() { // Call this function before calc if
                 // have -ve weights
    for(int i = 0; i < N; i++) {
      for(int e = 0; e < eds.size(); e++) {
        const Edge &E = eds[e]; // Bellman-Ford
        if(E.cap)
          ckmin(pi[E.to], pi[eds[e ^ 1].to] + E.cost);
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
        pi[i] += dist[i]; // don't matter for
                          // unreachable nodes
      F df = f;
      for(int x = t; x != s; x = eds[pre[x] ^ 1].to) {
        const Edge &E = eds[pre[x]];
        ckmin(df, E.cap - E.flo);
      }
      f -= df;
      totFlow += df;
      totCost += (pi[t] - pi[s]) * df;
      for(int x = t; x != s; x = eds[pre[x] ^ 1].to)
        eds[pre[x]].flo += df, eds[pre[x] ^ 1].flo
                               -= df;
    } // get max flow you can send along path
    return {totFlow, totCost};
  }
};