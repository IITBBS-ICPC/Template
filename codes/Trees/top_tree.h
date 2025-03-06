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

// node combine, upd combine and upd apply definitions
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

struct node {
  int par, child[4];
  Data path, sub, all, data;
  Upd plazy, slazy;
  bool flip, fake;
  node()
      : par(0), child(), path(), sub(), all(), plazy(),
        slazy(), flip(false), fake(true) {}
  node(int v) : node() {
    data = Data(v);
    path = all = Data(data);
    fake = false;
  }
};
// Splay Tree
struct SplayTree {
  vector<node> t;
  SplayTree(int n) : t(n) {}
  void pushflip(int u) {
    if(!u) return;
    swap(t[u].child[0], t[u].child[1]);
    t[u].flip ^= true;
  }
  void pushpath(int u, const Upd &upd) {
    if(!u || t[u].fake) return;
    t[u].data += upd;
    t[u].path += upd;
    t[u].all = t[u].path + t[u].sub;
    t[u].plazy += upd;
  }
  void pushsub(int u, bool tag, const Upd &upd) {
    if(!u) return;
    t[u].sub += upd;
    t[u].slazy += upd;
    if(!t[u].fake && tag) pushpath(u, upd);
    else
      t[u].all = t[u].path + t[u].sub;
  }
  void push(int u) {
    if(!u) return;
    if(t[u].flip) {
      pushflip(t[u].child[0]);
      pushflip(t[u].child[1]);
      t[u].flip = false;
    }
    if(t[u].plazy.upd()) {
      pushpath(t[u].child[0], t[u].plazy);
      pushpath(t[u].child[1], t[u].plazy);
      t[u].plazy = Upd();
    }
    if(t[u].slazy.upd()) {
      pushsub(t[u].child[0], false, t[u].slazy);
      pushsub(t[u].child[1], false, t[u].slazy);
      pushsub(t[u].child[2], true, t[u].slazy);
      pushsub(t[u].child[3], true, t[u].slazy);
      t[u].slazy = Upd();
    }
  }
  void pull(int u) {
    if(!t[u].fake)
      t[u].path = t[t[u].child[0]].path + t[u].data
                  + t[t[u].child[1]].path;
    t[u].sub
      = t[t[u].child[0]].sub + t[t[u].child[1]].sub
        + t[t[u].child[2]].all + t[t[u].child[3]].all;
    t[u].all = t[u].path + t[u].sub;
  }
  void attach(int u, int d, int v) {
    t[u].child[d] = v;
    t[v].par = u;
    pull(u);
  }
  int dir(int u, int tag) {
    int v = t[u].par;
    return t[v].child[tag] == u       ? tag
           : t[v].child[tag + 1] == u ? tag + 1
                                    : -1;
  }
  void rotate(int u, int tag) {
    int v = t[u].par, w = t[v].par, du = dir(u, tag),
        dv = dir(v, tag);
    if(dv == -1 && tag == 0) dv = dir(v, 2);
    attach(v, du, t[u].child[du ^ 1]);
    attach(u, du ^ 1, v);
    if(~dv) attach(w, dv, u);
    else
      t[u].par = w;
  }
  void splay(int u, int tag) {
    push(u);
    while(~dir(u, tag) && (tag == 0 || t[t[u].par].fake)) {
      int v = t[u].par, w = t[v].par;
      push(w);
      push(v);
      push(u);
      int du = dir(u, tag), dv = dir(v, tag);
      if(~dv && (tag == 0 || t[w].fake))
        rotate(du == dv ? v : u, tag);
      rotate(u, tag);
    }
  }
};
// Fully Dynamic Tree
struct LinkCut : SplayTree {
  vector<int> fakes;
  LinkCut(int n) : SplayTree(2 * n + 1) {
    for(int i = 1; i <= 2 * n; i++) {
      if(i <= n) t[i].fake = false;
      else
        fakes.push_back(i);
    }
  }
  void add(int u, int v) {
    if(!v) return;
    for(int i = 2; i < 4; i++)
      if(!t[u].child[i]) {
        attach(u, i, v);
        return;
      }
    int w = fakes.back();
    fakes.pop_back();
    attach(w, 2, t[u].child[2]);
    attach(w, 3, v);
    attach(u, 2, w);
  }
  void recPush(int u) {
    if(t[u].fake) recPush(t[u].par);
    push(u);
  }
  void rem(int u) {
    int v = t[u].par;
    recPush(v);
    if(t[v].fake) {
      int w = t[v].par;
      attach(w, dir(v, 2), t[v].child[dir(u, 2) ^ 1]);
      if(t[w].fake) splay(w, 2);
      fakes.push_back(v);
    } else {
      attach(v, dir(u, 2), 0);
    }
    t[u].par = 0;
  }
  int par(int u) {
    int v = t[u].par;
    if(!t[v].fake) return v;
    splay(v, 2);
    return t[v].par;
  }
  int access(int u) {
    int v = u;
    splay(u, 0), add(u, t[u].child[1]), attach(u, 1, 0);
    while(t[u].par) {
      v = par(u);
      splay(v, 0);
      rem(u);
      add(v, t[v].child[1]);
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
    t[v].child[0] = t[u].par = 0;
    pull(v);
  }
  Data getPath(int u, int v) {
    reroot(u);
    access(v);
    return t[v].path;
  }
  void updatePath(int u, int v, Upd upd) {
    reroot(u);
    access(v);
    pushpath(v, upd);
  }
  Data getSubtree(int v) {
    access(v);
    Data ret = t[v].data;
    for(int i = 2; i < 4; i++)
      ret += t[t[v].child[i]].all;
    return ret;
  }
  void updateSubtree(int v, Upd upd) {
    access(v);
    t[v].data += upd;
    for(int i = 2; i < 4; i++)
      pushsub(t[v].child[i], true, upd);
  }
  int lca(int u, int v) {
    if(u == v) return u;
    access(u);
    int ret = access(v);
    return t[u].par ? ret : 0;
  }
};
// Balanced Binary Search Tree
struct BBST : public SplayTree {
  int n;
  int root;
  BBST(int n) : SplayTree(n + 5), n(n) {
    for(auto &x : t)
      x.fake = false;
  }
  void build(vector<int> &arr) {
    for(int i = n, v = n; i; i--, v--) {
      t[v].data = Data(arr[i]);
      if(i < n - 1) {
        t[v].child[1] = v + 1, t[v + 1].par = v;
        pull(v);
      }
    }
    t[root = n + 2].child[1] = 1, t[1].par = n + 2;
  }
  int find(int i) {
    int v = t[root].child[1];
    while(true) {
      int l = t[v].child[0], tag = t[v].child[1];
      if(t[l].path.n >= i) v = l;
      else if((i -= t[l].path.n) == 1)
        break;
      else
        v = tag, --i;
    }
    return v;
  }
  void subtreeSplay(int x, int tag) {
    int par = t[tag].par, d = dir(tag, 0);
    if(~d) t[tag].par = 0;
    splay(x, 0);
    if(~d) attach(par, d, x);
  }
  pair<int, int> compressRange(int l, int tag) {
    int vl = find(l - 1), vr = find(tag + 1);
    subtreeSplay(vl, t[root].child[1]);
    subtreeSplay(vr, t[vl].child[1]);
    return {vl, vr};
  }
  Data query(int l, int tag) {
    auto [vl, vr] = compressRange(l, tag);
    return t[t[vr].child[0]].path;
  }
  void update(int l, int tag, Upd upd) {
    auto [vl, vr] = compressRange(l, tag);
    if(int u = t[vr].child[0]) {
      pushpath(u, upd);
      pull(vr);
      pull(vl);
    }
  }
  void flip(int l, int tag) {
    auto [vl, vr] = compressRange(l, tag);
    if(int u = t[vr].child[0]) pushflip(u);
  }
};
