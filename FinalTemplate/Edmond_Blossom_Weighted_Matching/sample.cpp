#include<bits/stdc++.h>
using namespace std;

#include "template.h"

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    Blossom g(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        g.add_edge(u-1, v-1, w); // pass 0-based index here.
    }
    auto ans = g.solve();  // {max_weight, matching size}
    cout << ans.second << ' ' << ans.first << '\n';
    for (int i = 0; i < n; ++i) {
        if (g.match[i] > i) cout << i+1 << ' ' << g.match[i]+1 << '\n';     // print 1-based index.
    }
}