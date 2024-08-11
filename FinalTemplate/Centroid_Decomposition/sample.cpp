#include<bits/stdc++.h>
using namespace std;

#include "template.h"

// Based on CSES problem "Finding a Centroid", submission link in README

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