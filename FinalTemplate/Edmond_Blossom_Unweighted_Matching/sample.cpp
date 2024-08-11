#include<bits/stdc++.h>
using namespace std;

#include "template.h"

int main() {
    int n,m;
    cin >> n >> m; 
    edmond er(n, m);
    while(m--) {
        int x,y; cin >> x >> y;
        er.addEdge(x+1, y+1); // Input should be strictly 1-based indexed node.
    }
    er.calc_max_match();
    cout << er.n_matches << endl;
    for(int u = 1;u <= er.n; ++u)
        if(er.mate[u] > u) cout << er.mate[u]-1 << ' ' << u-1 << '\n';
    return 0;
}