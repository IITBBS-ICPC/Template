// Problem: https://cses.fi/problemset/task/2079
// Submission : https://cses.fi/paste/d5cb294c0b2815db9aef6c/

#include<bits/stdc++.h>
using namespace std;

const int N=200005; // Change based on constraint
set<int> G[N]; // adjacency list (note that this is stored in set, not vector)
int sz[N], pa[N];

int dfs(int u, int p) {
  sz[u] = 1;
  for(auto v : G[u]) if(v != p) {
    sz[u] += dfs(v, u);
  }
  return sz[u];
}
int centroid(int u, int p, int n) {
  for(auto v : G[u]) if(v != p) {
    if(sz[v] > n / 2) return centroid(v, u, n);
  }
  return u;
}
void build(int u, int p) {
  int n = dfs(u, p);
  int c = centroid(u, p, n);
  if(p == -1) p = c;
  pa[c] = p;

  vector<int> tmp(G[c].begin(), G[c].end());
  for(auto v : tmp) {
    G[c].erase(v); G[v].erase(c);
    build(v, c);
  }
}

int main() {
    int n; // Number of nodes in the tree
    cin >> n;

    // Read the edges
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        G[u].insert(v);
        G[v].insert(u);
    }

    // To build the centroid tree starting from node 1 with no parent (-1)
    // build(1, -1);

    // To Output the parent of each node in the centroid tree
    // for(int i = 1; i <= n; i++) {
    //     cout << "Parent of node " << i << " in centroid tree is " << pa[i] << endl;
    // }

    // To output centroid

    int total_size=dfs(1,-1);

    cout<<centroid(1,-1,total_size);

    return 0;
}