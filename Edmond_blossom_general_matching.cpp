#include<bits/stdc++.h>
using namespace std;

struct edmond {

    
    int n, m , nE, n_matches,q_n,book_mark;
    vector<int> adj,nxt,go,mate,q,book,type,fa,bel;

    edmond(int n,int m):n(n),m(m){
        nE=0;
        n_matches=0;
        adj.resize(n+1);
        mate.resize(n+1);
        q.resize(n+1);
        book.resize(n+1);
        type.resize(n+1);
        fa.resize(n+1);
        bel.resize(n+1);
        go.resize((m<<1)|1);
        nxt.resize((m<<1)|1);
    }

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
