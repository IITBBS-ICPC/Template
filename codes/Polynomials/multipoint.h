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

vi eval(vi &p, vi &pts) {
  cap = false;
  int n = pts.size();
  vi tree[4 * n + 1];
  function<void(int, int, int)> build =
    [&](int v, int l, int r) {
      if(l == r) {
        tree[v] = {(M - pts[l]) % M, 1};
      } else {
        int m = (l + r) / 2;
        build(2 * v, l, m), build(2 * v + 1, m + 1, r);
        tree[v] = tree[2 * v] * tree[2 * v + 1];
      }
    };
  build(1, 0, n - 1);
  vi ans(n, 0);
  function<void(int, int, int, vi)> evaluate
    = [&](int v, int l, int r, vi curr) {
        trim(curr);
        if(r - l <= 32) {
          for(int i = l; i <= r; i++) {
            ans[i] = eval(curr, pts[i]);
          }
        } else {
          int m = (l + r) / 2;
          evaluate(2 * v, l, m, curr % tree[2 * v]);
          evaluate(2 * v + 1, m + 1, r,
                   curr % tree[2 * v + 1]);
        }
      };
  evaluate(1, 0, n - 1, p % tree[1]);
  cap = true;
  return ans;
}
