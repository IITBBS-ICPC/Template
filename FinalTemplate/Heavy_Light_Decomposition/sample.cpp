#include<bits/stdc++.h>
using namespace std;
#include "template.h"

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, q;
    cin >> n >> q;

    vector<int> v(n);
    for (int i = 0;i < n;i++) {
        cin >> v[i];
    }

    vector<vector<int>> adj(n);
    for (int i = 0;i < n - 1;i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    HLD h(adj);
    for (int i = 0;i < n;i++) {
        h.update(i, v[i]);
    }

    for (int i = 0;i < q;i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int s, x;
            cin >> s >> x;
            s--;
            h.update(s, x);
        }
        else {
            int a, b;
            cin >> a >> b;
            a--, b--;
            cout << h.query(a, b) << " ";
        }
    }
    cout << endl;
}