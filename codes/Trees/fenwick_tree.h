struct FenwickTree {
  int n, m;
  vector<vector<int>> bit;
  void build(int _n, int _m) {
    n = _n, m = _m;
    bit.assign(n, vector<int>(m, 0));
  }
  int query(int x, int y) {
    int ans = 0;
    for(; x; x -= x & -x)
      for(int j = y; j; j -= j & -j)
        ans += bit[x][j];
    return ans;
  }
  void update(int x, int y, int val) {
    for(; x < n; x += x & -x)
      for(int j = y; j < m; j += j & -j)
        bit[x][j] += val;
  }
  int query(int x1, int y1, int x2, int y2) {
    return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1)
           + query(x1 - 1, y1 - 1);
  }
};
