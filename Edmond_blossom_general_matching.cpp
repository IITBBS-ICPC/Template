#include<bits/stdc++.h>
using namespace std;

namespace edmond {

    const int MaxN = 501;
    const int MaxM = MaxN * (MaxN - 1);
    
    int n, m;
    int nE, adj[MaxN];
    int nxt[MaxM], go[MaxM];
    int n_matches;
    int mate[MaxN];

    int q_n;
    int q[MaxN];
    int book_mark;
    int book[MaxN];

    int type[MaxN];
    int fa[MaxN];
    int bel[MaxN];

    void addEdge(const int &u, const int &v) {
        nxt[++nE] = adj[u], go[adj[u] = nE] = v;
        nxt[++nE] = adj[v], go[adj[v] = nE] = u;
    }

    void augment(int u) {
        while (u) {
            int nu = mate[fa[u]];
            mate[mate[u] = fa[u]] = u;
            u = nu;
        }
    }

    int get_lca(int u, int v) {
        ++book_mark;
        while (true) {
            if (u) {
                if (book[u] == book_mark) return u;
                book[u] = book_mark;
                u = bel[fa[mate[u]]];
            }
            swap(u, v);
        }
    }

    void go_up(int u, int v, const int &mv) {
        while (bel[u] != mv) {
            fa[u] = v;
            v = mate[u];
            if (type[v] == 1) type[q[++q_n] = v] = 0;
            bel[u] = bel[v] = mv;
            u = fa[v];
        }
    }
    void after_go_up() {
        for (int u = 1; u <= n; ++u) bel[u] = bel[bel[u]];
    }

    bool match(const int &sv) {
        for (int u = 1; u <= n; ++u) bel[u] = u, type[u] = -1;
        type[q[q_n = 1] = sv] = 0;
        for (int i = 1; i <= q_n; ++i) {
            int u = q[i];
            for (int e = adj[u]; e; e = nxt[e]) {
                int v = go[e];
                if (!~type[v]) {
                    fa[v] = u, type[v] = 1;
                    int nu = mate[v];
                    if (!nu) {
                        augment(v);
                        return true;
                    }
                    type[q[++q_n] = nu] = 0;
                } else if (!type[v] && bel[u] != bel[v]) {
                    int lca = get_lca(u, v);
                    go_up(u, v, lca);
                    go_up(v, u, lca);
                    after_go_up();
                }
            }
        }
        return false;
    }

    void calc_max_match() {
        n_matches = 0;
        for (int u = 1; u <= n; ++u) 
            if (!mate[u] && match(u)) ++n_matches;
    }
};

int main() {
    cin >> edmond::n >> edmond::m;
    while(edmond::m--) {
        int x,y; cin >> x >> y;
        edmond::addEdge(x, y); // Input should be strictly 1-based indexed node.
    }
    edmond::calc_max_match();
    cout << edmond::n_matches << endl;
    for(int u = 1;u <= edmond::n; ++u)
        if(edmond::mate[u] > u) cout << edmond::mate[u] << ' ' << u << '\n';
    return 0;
}
