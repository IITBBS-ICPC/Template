void trim(vi &a) {
  while(a.size() && !a.back())
    a.pop_back();
}
int eval(vi &p, int pt) {
  int ans = 0;
  for(int i = 0, b = 1; i < p.size();
      i++, b = (b * 1ll * pt) % M)
    ans = (ans + (b * 1ll * p[i]) % M) % M;
  return ans;
}
vi interpolate(vi &x, vi &y) {
  cap = false;
  int n = x.size();
  vi tree[4 * n + 1];
  function<void(int, int, int)> build =
    [&](int v, int l, int r) {
      if(l == r) {
        tree[v] = {(M - x[l]) % M, 1};
      } else {
        int m = (l + r) / 2;
        build(2 * v, l, m), build(2 * v + 1, m + 1, r);
        tree[v] = tree[2 * v] * tree[2 * v + 1];
      }
    };
  build(1, 0, n - 1);
  vi p = tree[1];
  vi den(n);
  deriv(p);
  function<vi(int, int, int, vi)> evaluate =
    [&](int v, int l, int r, vi curr) {
      trim(curr);
      if(l == r) {
        den[l] = eval(curr, x[l]);
        return vi({mod_div(y[l], den[l])});
      } else {
        int m = (l + r) / 2;
        vi yl
          = evaluate(2 * v, l, m, curr % tree[2 * v]);
        vi yr = evaluate(
          2 * v + 1, m + 1, r, curr % tree[2 * v + 1]);
        return yl * tree[2 * v + 1] + yr * tree[2 * v];
      }
    };
  return evaluate(1, 0, n - 1, p % tree[1]);
}