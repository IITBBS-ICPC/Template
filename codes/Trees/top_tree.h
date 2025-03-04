// Value of each node of the tree
struct Val {
  int v, vp;
  Val() : v(0), vp(0) {}
  Val(int v, int vp) : v(v), vp(vp) {}
};
// Lazy tags
struct Lazy {
  bool flip;
  Lazy(bool flip = 0) : flip(flip) {}
  bool lazy() { return flip; }
};

// Node combine, lazy combine and lazy apply definitions
Val operator+(const Val &a, const Val &b) {
  return Val(a.v + b.v, a.vp + b.vp);
}
Val &operator+=(Val &a, const Val &b) {
  return a = a + b;
}
Lazy &operator+=(Lazy &a, const Lazy &b) {
  return a = Lazy(a.flip ^ b.flip);
}
Val &operator+=(Val &a, const Lazy &b) {
  if(b.flip) swap(a.v, a.vp);
  return a;
}
struct Node {
  int p, ch[4];
  Val path, sub, all, val;
  Lazy plazy, slazy;
  bool flip, fake;
  Node()
      : p(0), ch(), path(), sub(), all(), plazy(),
        slazy(), flip(false), fake(true) {}
  Node(int v, int vp) : Node() {
    val = Val(v, vp);
    path = all = Val(val);
    fake = false;
  }
};
// Splay Tree
struct SplayTree {
  vector<Node> T;
  SplayTree(int n) : T(n) {}
  void pushFlip(int u) {
    if(!u) return;
    swap(T[u].ch[0], T[u].ch[1]);
    T[u].flip ^= true;
  }
  void pushPath(int u, const Lazy &lazy) {
    if(!u || T[u].fake) return;
    T[u].val += lazy;
    T[u].path += lazy;
    T[u].all = T[u].path + T[u].sub;
    T[u].plazy += lazy;
  }
  void pushSub(int u, bool o, const Lazy &lazy) {
    if(!u) return;
    T[u].sub += lazy;
    T[u].slazy += lazy;
    if(!T[u].fake && o) pushPath(u, lazy);
    else
      T[u].all = T[u].path + T[u].sub;
  }
  void push(int u) {
    if(!u) return;
    if(T[u].flip) {
      pushFlip(T[u].ch[0]);
      pushFlip(T[u].ch[1]);
      T[u].flip = false;
    }
    if(T[u].plazy.lazy()) {
      pushPath(T[u].ch[0], T[u].plazy);
      pushPath(T[u].ch[1], T[u].plazy);
      T[u].plazy = Lazy();
    }
    if(T[u].slazy.lazy()) {
      pushSub(T[u].ch[0], false, T[u].slazy);
      pushSub(T[u].ch[1], false, T[u].slazy);
      pushSub(T[u].ch[2], true, T[u].slazy);
      pushSub(T[u].ch[3], true, T[u].slazy);
      T[u].slazy = Lazy();
    }
  }
  void pull(int u) {
    if(!T[u].fake)
      T[u].path = T[T[u].ch[0]].path + T[u].val
                  + T[T[u].ch[1]].path;
    T[u].sub = T[T[u].ch[0]].sub + T[T[u].ch[1]].sub
               + T[T[u].ch[2]].all + T[T[u].ch[3]].all;
    T[u].all = T[u].path + T[u].sub;
  }
  void attach(int u, int d, int v) {
    T[u].ch[d] = v;
    T[v].p = u;
    pull(u);
  }
  int dir(int u, int o) {
    int v = T[u].p;
    return T[v].ch[o] == u       ? o
           : T[v].ch[o + 1] == u ? o + 1
                                 : -1;
  }
  void rotate(int u, int o) {
    int v = T[u].p, w = T[v].p, du = dir(u, o),
        dv = dir(v, o);
    if(dv == -1 && o == 0) dv = dir(v, 2);
    attach(v, du, T[u].ch[du ^ 1]);
    attach(u, du ^ 1, v);
    if(~dv) attach(w, dv, u);
    else
      T[u].p = w;
  }
  void splay(int u, int o) {
    push(u);
    while(~dir(u, o) && (o == 0 || T[T[u].p].fake)) {
      int v = T[u].p, w = T[v].p;
      push(w);
      push(v);
      push(u);
      int du = dir(u, o), dv = dir(v, o);
      if(~dv && (o == 0 || T[w].fake))
        rotate(du == dv ? v : u, o);
      rotate(u, o);
    }
  }
};
// Fully Dynamic Tree
struct LinkCut : SplayTree {
  vector<int> freeList;
  LinkCut(int n) : SplayTree(2 * n + 1) {
    for(int i = 1; i <= 2 * n; i++) {
      if(i <= n) T[i].fake = false;
      else
        freeList.push_back(i);
    }
  }
  void add(int u, int v) {
    if(!v) return;
    for(int i = 2; i < 4; i++)
      if(!T[u].ch[i]) {
        attach(u, i, v);
        return;
      }
    int w = freeList.back();
    freeList.pop_back();
    attach(w, 2, T[u].ch[2]);
    attach(w, 3, v);
    attach(u, 2, w);
  }
  void recPush(int u) {
    if(T[u].fake) recPush(T[u].p);
    push(u);
  }
  void rem(int u) {
    int v = T[u].p;
    recPush(v);
    if(T[v].fake) {
      int w = T[v].p;
      attach(w, dir(v, 2), T[v].ch[dir(u, 2) ^ 1]);
      if(T[w].fake) splay(w, 2);
      freeList.push_back(v);
    } else {
      attach(v, dir(u, 2), 0);
    }
    T[u].p = 0;
  }
  int par(int u) {
    int v = T[u].p;
    if(!T[v].fake) return v;
    splay(v, 2);
    return T[v].p;
  }
  int access(int u) {
    int v = u;
    splay(u, 0), add(u, T[u].ch[1]), attach(u, 1, 0);
    while(T[u].p) {
      v = par(u);
      splay(v, 0);
      rem(u);
      add(v, T[v].ch[1]);
      attach(v, 1, u);
      splay(u, 0);
    }
    return v;
  }
  void reroot(int u) {
    access(u);
    pushFlip(u);
  }
  void link(int u, int v) {
    reroot(u);
    access(v);
    add(v, u);
  }
  void cut(int u, int v) {
    reroot(u);
    access(v);
    T[v].ch[0] = T[u].p = 0;
    pull(v);
  }
  Val getPath(int u, int v) {
    reroot(u);
    access(v);
    return T[v].path;
  }
  void updatePath(int u, int v, Lazy lz) {
    reroot(u);
    access(v);
    pushPath(v, lz);
  }
  Val getSubtree(int v) {
    access(v);
    Val ret = T[v].val;
    for(int i = 2; i < 4; i++)
      ret += T[T[v].ch[i]].all;
    return ret;
  }
  void updateSubtree(int v, Lazy lz) {
    access(v);
    T[v].val += lz;
    for(int i = 2; i < 4; i++)
      pushSub(T[v].ch[i], true, lz);
  }
  int lca(int u, int v) {
    if(u == v) return u;
    access(u);
    int ret = access(v);
    return T[u].p ? ret : 0;
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
      T[v].val = Val(arr[i]);
      if(i < n - 1) {
        T[v].ch[1] = v + 1, T[v + 1].p = v;
        pull(v);
      }
    }
    T[root = n + 2].ch[1] = 1, T[1].p = n + 2;
  }
  int find(int i) {
    int v = T[root].ch[1];
    while(true) {
      int l = T[v].ch[0], r = T[v].ch[1];
      if(T[l].path.n >= i) v = l;
      else if((i -= T[l].path.n) == 1)
        break;
      else
        v = r, --i;
    }
    return v;
  }
  void subtreeSplay(int x, int r) {
    int p = T[r].p, d = dir(r, 0);
    if(~d) T[r].p = 0;
    splay(x, 0);
    if(~d) attach(p, d, x);
  }
  pair<int, int> compressRange(int l, int r) {
    int vl = find(l - 1), vr = find(r + 1);
    subtreeSplay(vl, T[root].ch[1]);
    subtreeSplay(vr, T[vl].ch[1]);
    return {vl, vr};
  }
  Val query(int l, int r) {
    auto [vl, vr] = compressRange(l, r);
    return T[T[vr].ch[0]].path;
  }
  void update(int l, int r, Lazy upd) {
    auto [vl, vr] = compressRange(l, r);
    if(int u = T[vr].ch[0]) {
      pushPath(u, upd);
      pull(vr);
      pull(vl);
    }
  }
  void flip(int l, int r) {
    auto [vl, vr] = compressRange(l, r);
    if(int u = T[vr].ch[0]) pushFlip(u);
  }
};
