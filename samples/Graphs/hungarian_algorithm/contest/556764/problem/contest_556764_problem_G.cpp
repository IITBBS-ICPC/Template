// WEEK 5 contest -> Problem G: 
// problem: https://codeforces.com/group/wlb0UYQSQF/contest/556764/problem/G
// submission: https://codeforces.com/group/wlb0UYQSQF/contest/556764/submission/289426972
#include <bits/stdc++.h>

using namespace std;

using lld = double;
const lld inf = 1e18;
struct Hungarian {
  int n, m;
  vector<int> p;
  vector<lld> u, v, way;
  vector<vector<lld>> A;
  Hungarian(vector<vector<lld>> &arr) {
    n = arr.size() - 1;
    m = arr[0].size() - 1;
    A = arr;
    u.resize(n + 1);
    v.resize(m + 1);
    p.resize(m + 1);
    way.resize(m + 1);
  }
  void calc() {
    for(int i = 1; i <= n; ++i) {
      // cout << i << endl;
      p[0] = i;
      int j0 = 0;
      vector<lld> minv(m + 1, inf);
      vector<bool> used(m + 1, false);
      do {
        used[j0] = true;
        int i0 = p[j0], j1;
        lld delta = inf;
        for(int j = 1; j <= m; ++j) {
          if(!used[j]) {
            // cout << "T " << i0 << ' ' << j  << endl;
            lld cur = A[i0][j] - u[i0] - v[j];
            if(cur < minv[j])
              minv[j] = cur, way[j] = j0;
            if(minv[j] < delta)
              delta = minv[j], j1 = j;
          }
        }
        for(int j = 0; j <= m; ++j)
          if(used[j])
            u[p[j]] += delta, v[j] -= delta;
          else
            minv[j] -= delta;
        j0 = j1;
      } while(p[j0] != 0);
      do {
        int j1 = way[j0];
        p[j0] = p[j1];
        j0 = j1;
      } while(j0);
    }
  }

  vector<int> getAssignment() {
    vector<int> ans(n + 1);
    for(int j = 1; j <= m; ++j)
      ans[p[j]] = j;
    return ans;
  }

  lld mncost() { return -v[0]; }
};

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n;
  cin >> n;
  vector<vector<lld>> arr(n + 1, vector<lld>(n + 1));
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      int t; cin >> t;
      arr[i + 1][j + 1] =  - logl(t);
      // cout << arr[i + 1][j + 1] << ' ';
    }
  }
  Hungarian h(arr);
  h.calc();
  // return 0;

  auto ans = h.p;
  auto mnCost = h.mncost();
  // cout << mnCost << '\n';
  for(int i = 1; i <= n; i++) {
    cout << ans[i] << ' ';
  }


  return 0;
}