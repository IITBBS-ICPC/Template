struct node {
  int val, l, r;
  node(int _v = 0) : val(_v), l(0), r(0) {}
  static node combine(node &l, node &r) { return node(l.val + r.val); }
};
const int N = 1e5 + 10;
const int LG = __lg(N);
int tp = 0;
node t[N * LG];
int createNode(int _v = 0) {
  t[tp] = node(_v);
  return tp++;
}
struct SegmentTree {
  int n;
  int duplicate(int v) {
    int r = createNode();
    t[r] = t[v];
    return r;
  }
  void pull(int v) { t[v].val = t[t[v].l].val + t[t[v].r].val; }
  int build(int l, int r, vi &arr) {
    if(l == r) {
      return createNode(arr[l]);
    } else {
      int v = createNode();
      int m = (l + r) / 2;
      t[v].l = build(l, m, arr);
      t[v].r = build(m + 1, r, arr);
      pull(v);
      return v;
    }
  }
  int build(vi &arr) {
    n = arr.size();
    return build(0, n - 1, arr);
  }
  int update(int v, int l, int r, int ind, int val) {
    if(l == r) {
      v = createNode(val);
      return v;
    } else {
      int nv = createNode();
      t[nv] = t[v];
      int m = (l + r) / 2;
      if(ind <= m)
        t[nv].l = update(t[v].l, l, m, ind, val);
      else
        t[nv].r = update(t[v].r, m + 1, r, ind, val);
      pull(nv);
      return nv;
    }
  }
  node query(int v, int tl, int tr, int l, int r) {
    if(tl > r || tr < l)
      return node();
    else if(tl >= l && tr <= r)
      return t[v];
    else {
      int tm = (tl + tr) / 2;
      auto ql = query(t[v].l, tl, tm, l, r);
      auto qr = query(t[v].r, tm + 1, tr, l, r);
      return node::combine(ql, qr);
    }
  }
  node query(int v, int l, int r) { return query(v, 0, n - 1, l, r); }
  int update(int v, int ind, int val) { return update(v, 0, n - 1, ind, val); }
};