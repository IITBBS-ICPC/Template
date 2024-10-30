#include <bits/stdc++.h>

using namespace std;

// Required additional data structures
struct node {
  node(int v);
  static node combine(node &a, node &b);
};
struct SegmentTree {
  void build(vector<int> &arr);
  node query(int v, int tl, int tr, int l, int r);
  node query(int l, int r);
  void update(int v, int tl, int tr, int l, int r, int x);
  void update(int l, int r, int x);
};

struct HLD {
  vector<int> ord;
  vector<int> pos;
  vector<int> head;
  vector<int> par;
  vector<int> depth;
  vector<int> heavy;
  SegmentTree st;

  void build(vector<vector<int>> &g, vector<int> &a) {
    int n = g.size() - 1;
    pos.assign(n + 1, 0), head.assign(n + 1, 0), par.assign(n + 1, 0),
      depth.assign(n + 1, 0), heavy.assign(n + 1, 0);

    function<int(int, int)> dfs = [&](int v, int p) {
      int sz = 1;
      int mxcsz = 0;
      for(auto c : g[v]) {
        if(c != p) {
          par[c] = v;
          depth[c] = depth[v] + 1;

          int csz = dfs(c, v);

          sz += csz;
          if(csz > mxcsz) {
            heavy[v] = c;
            mxcsz = csz;
          }
        }
      }
      return sz;
    };

    function<void(int, int, int)> decomp = [&](int v, int p, int h) {
      head[v] = h;
      ord.push_back(a[v]);
      pos[v] = ord.size() - 1;

      if(heavy[v])
        decomp(heavy[v], v, h);
      for(auto c : g[v]) {
        if(c != p && c != heavy[v]) {
          decomp(c, v, c);
        }
      }
    };

    dfs(1, 0);
    decomp(1, 0, 1);
    st.build(ord);
  }

  int query(int u, int v) {
    node resu, resv;
    for(; head[u] != head[v]; v = par[head[v]]) {
      if(depth[head[u]] > depth[head[v]]) {
        swap(u, v);
        swap(resu, resv);
      }
      node q = st.query(pos[head[v]], pos[v]);
      resv = node::combine(q, resv);
    }
    if(depth[u] > depth[v]) {
      swap(u, v);
      swap(resu, resv);
    }

    node q = st.query(pos[u], pos[v]);
    resv = node::combine(q, resv);

    // resv stores query LCA to v
    // resu stores query form LCA to u
    // lower depth side is considered left in each query, process accordingly

    int res; // define combination of lca - node queries
    return res;
  }

  void update(int u, int v, int x) {
    for(; head[u] != head[v]; v = par[head[v]]) {
      if(depth[head[u]] > depth[head[v]]) {
        swap(u, v);
      }
      st.update(pos[head[v]], pos[v], x);
    }
    if(depth[u] > depth[v]) {
      swap(u, v);
    }
    st.update(pos[u], pos[v], x);
  }
};

