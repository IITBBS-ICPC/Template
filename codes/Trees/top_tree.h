#define opr operator
struct Data {
  int v;
  Data() : v(0) {}
  Data(int v) : v(v) {}
};

struct Upd {
  int v;
  Upd(int v = 0) : v(v) {}
  bool upd() { return v; }
};

// Node combine, upd combine and upd apply definitions
Data opr + (const Data &a, const Data &b) {
  return Data(a.v + b.v);
}
Data &opr += (Data & a, const Data &b) {
  return a = a + b;
}
Upd &opr += (Upd & a, const Upd &b) {
  return a = Upd(max(a.v, b.v));
}
Data &opr += (Data & a, const Upd &b) {
  return a = Data(a.v + b.v);
}

struct Node {
  int par, child[4];
  Data path, sub, all, data;
  Update plazy, slazy;
  bool flip, fake;
  Node()
      : par(0), child(), path(), sub(), all(), plazy(),
        slazy(), flip(false), fake(true) {}
  Node(int v, int vp) : Node() {
    data = Data(v, vp);
    path = all = Data(data);
    fake = false;
  }
};
// Splay Tree
struct SplayTree {
  vector<Node> T;
  SplayTree(int n) : T(n) {}
  void pushflip(int u) {
    if(!u) return;
    swap(T[u].child[0], T[u].child[1]);
    T[u].flip ^= true;
  }
  void pushpath(int u, const Update &upd) {
    if(!u || T[u].fake) return;
    T[u].data += upd;
    T[u].path += upd;
    T[u].all = T[u].path + T[u].sub;
    T[u].plazy += upd;
  }
  void pushsub(int u, bool r, const Update &upd) {
    if(!u) return;
    T[u].sub += upd;
    T[u].slazy += upd;
    if(!T[u].fake && r) pushpath(u, upd);
    else
      T[u].all = T[u].path + T[u].sub;
  }
  void push(int u) {
    if(!u) return;
    if(T[u].flip) {
      pushflip(T[u].child[0]);
      pushflip(T[u].child[1]);
      T[u].flip = false;
    }
    if(T[u].plazy.upd()) {
      pushpath(T[u].child[0], T[u].plazy);
      pushpath(T[u].child[1], T[u].plazy);
      T[u].plazy = Update();
    }
    if(T[u].slazy.upd()) {
      pushsub(T[u].child[0], false, T[u].slazy);
      pushsub(T[u].child[1], false, T[u].slazy);
      pushsub(T[u].child[2], true, T[u].slazy);
      pushsub(T[u].child[3], true, T[u].slazy);
      T[u].slazy = Update();
    }
  }
  void pull(int u) {
    if(!T[u].fake)
      T[u].path = T[T[u].child[0]].path + T[u].data
                  + T[T[u].child[1]].path;
    T[u].sub
      = T[T[u].child[0]].sub + T[T[u].child[1]].sub
        + T[T[u].child[2]].all + T[T[u].child[3]].all;
    T[u].all = T[u].path + T[u].sub;
  }
  void attach(int u, int d, int v) {
    T[u].child[d] = v;
    T[v].par = u;
    pull(u);
  }
  int dir(int u, int r) {
    int v = T[u].par;
    return T[v].child[r] == u       ? r
           : T[v].child[r + 1] == u ? r + 1
                                    : -1;
  }
  void rotate(int u, int r) {
    int v = T[u].par, w = T[v].par, du = dir(u, r),
        dv = dir(v, r);
    if(dv == -1 && r == 0) dv = dir(v, 2);
    attach(v, du, T[u].child[du ^ 1]);
    attach(u, du ^ 1, v);
    if(~dv) attach(w, dv, u);
    else
      T[u].par = w;
  }
  void splay(int u, int r) {
    push(u);
    while(~dir(u, r) && (r == 0 || T[T[u].par].fake)) {
      int v = T[u].par, w = T[v].par;
      push(w);
      push(v);
      push(u);
      int du = dir(u, r), dv = dir(v, r);
      if(~dv && (r == 0 || T[w].fake))
        rotate(du == dv ? v : u, r);
      rotate(u, r);
    }
  }
};
// Fully Dynamic Tree
struct LinkCut : SplayTree {
  vector<int> fakes;
  LinkCut(int n) : SplayTree(2 * n + 1) {
    for(int i = 1; i <= 2 * n; i++) {
      if(i <= n) T[i].fake = false;
      else
        fakes.push_back(i);
    }
  }
  void add(int u, int v) {
    if(!v) return;
    for(int i = 2; i < 4; i++)
      if(!T[u].child[i]) {
        attach(u, i, v);
        return;
      }
    int w = fakes.back();
    fakes.pop_back();
    attach(w, 2, T[u].child[2]);
    attach(w, 3, v);
    attach(u, 2, w);
  }
  void recPush(int u) {
    if(T[u].fake) recPush(T[u].par);
    push(u);
  }
  void rem(int u) {
    int v = T[u].par;
    recPush(v);
    if(T[v].fake) {
      int w = T[v].par;
      attach(w, dir(v, 2), T[v].child[dir(u, 2) ^ 1]);
      if(T[w].fake) splay(w, 2);
      fakes.push_back(v);
    } else {
      attach(v, dir(u, 2), 0);
    }
    T[u].par = 0;
  }
  int par(int u) {
    int v = T[u].par;
    if(!T[v].fake) return v;
    splay(v, 2);
    return T[v].par;
  }
  int access(int u) {
    int v = u;
    splay(u, 0), add(u, T[u].child[1]), attach(u, 1, 0);
    while(T[u].par) {
      v = par(u);
      splay(v, 0);
      rem(u);
      add(v, T[v].child[1]);
      attach(v, 1, u);
      splay(u, 0);
    }
    return v;
  }
  void reroot(int u) {
    access(u);
    pushflip(u);
  }
  void link(int u, int v) {
    reroot(u);
    access(v);
    add(v, u);
  }
  void cut(int u, int v) {
    reroot(u);
    access(v);
    T[v].child[0] = T[u].par = 0;
    pull(v);
  }
  Data getPath(int u, int v) {
    reroot(u);
    access(v);
    return T[v].path;
  }
  void updatePath(int u, int v, Update upd) {
    reroot(u);
    access(v);
    pushpath(v, upd);
  }
  Data getSubtree(int v) {
    access(v);
    Data ret = T[v].data;
    for(int i = 2; i < 4; i++)
      ret += T[T[v].child[i]].all;
    return ret;
  }
  void updateSubtree(int v, Update upd) {
    access(v);
    T[v].data += upd;
    for(int i = 2; i < 4; i++)
      pushsub(T[v].child[i], true, upd);
  }
  int lca(int u, int v) {
    if(u == v) return u;
    access(u);
    int ret = access(v);
    return T[u].par ? ret : 0;
  }
};
// Balanced Binary Search Tree
struct BBST : public SplayTree {
  int n;
  int root;
  BBST(int n) : SplayTree(n + 5), n(n) {
    for(auto &x : T)
      x.fake = false;
  }
  void build(vector<int> &arr) {
    for(int i = n, v = n; i; i--, v--) {
      T[v].data = Data(arr[i]);
      if(i < n - 1) {
        T[v].child[1] = v + 1, T[v + 1].par = v;
        pull(v);
      }
    }
    T[root = n + 2].child[1] = 1, T[1].par = n + 2;
  }
  int find(int i) {
    int v = T[root].child[1];
    while(true) {
      int l = T[v].child[0], r = T[v].child[1];
      if(T[l].path.n >= i) v = l;
      else if((i -= T[l].path.n) == 1)
        break;
      else
        v = r, --i;
    }
    return v;
  }
  void subtreeSplay(int x, int r) {
    int par = T[r].par, d = dir(r, 0);
    if(~d) T[r].par = 0;
    splay(x, 0);
    if(~d) attach(par, d, x);
  }
  pair<int, int> compressRange(int l, int r) {
    int vl = find(l - 1), vr = find(r + 1);
    subtreeSplay(vl, T[root].child[1]);
    subtreeSplay(vr, T[vl].child[1]);
    return {vl, vr};
  }
  Data query(int l, int r) {
    auto [vl, vr] = compressRange(l, r);
    return T[T[vr].child[0]].path;
  }
  void update(int l, int r, Update upd) {
    auto [vl, vr] = compressRange(l, r);
    if(int u = T[vr].child[0]) {
      pushpath(u, upd);
      pull(vr);
      pull(vl);
    }
  }
  void flip(int l, int r) {
    auto [vl, vr] = compressRange(l, r);
    if(int u = T[vr].child[0]) pushflip(u);
  }
};
