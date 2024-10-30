#include<bits/stdc++.h>
using namespace std;
// segment tree

#define int long long
const int segtree_size = 2e5 + 1;
vector<int> segtree(segtree_size << 1);
int combine(int lval, int rval) {
    return lval + rval;
}
void segtree_update(int v, int l, int r, int x, int val) {
    if (x < l || r < x)return;
    if (l == r) {
        segtree[v] = val;
        return;
    }
    int mid = (l + r) >> 1;
    int z = (mid + 1 - l) << 1;
    segtree_update(v + 1, l, mid, x, val);
    segtree_update(v + z, mid + 1, r, x, val);
    segtree[v] = combine(segtree[v + 1], segtree[v + z]);
}
int segtree_query(int v, int l, int r, int L, int R) {
    if (R < l || r < L)return 0;
    if (L <= l && r <= R) return segtree[v];
    int mid = (l + r) >> 1;
    int z = (mid + 1 - l) << 1;
    return combine(segtree_query(v + 1, l, mid, L, R), segtree_query(v + z, mid + 1, r, L, R));
}

int segment_tree_query(int L, int R) {
    int res = segtree_query(0, 0, segtree_size - 1, L, R);
    return res;
}

// HLD Template

// Taken from cp-algotithms

vector<int> parent, depth, heavy, head, pos;
int cur_pos;

int dfs(int v, vector<vector<int>> const& adj) {
    int size = 1;
    int max_c_size = 0;
    for (int c : adj[v]) {
        if (c != parent[v]) {
            parent[c] = v, depth[c] = depth[v] + 1;
            int c_size = dfs(c, adj);
            size += c_size;
            if (c_size > max_c_size)
                max_c_size = c_size, heavy[v] = c;
        }
    }
    return size;
}


int query(int a, int b) {
    int res = 0;
    for (; head[a] != head[b]; b = parent[head[b]]) {

        if (depth[head[a]] > depth[head[b]])
            swap(a, b);
        int cur_heavy_path_max = segment_tree_query(pos[head[b]], pos[b]);
        res = combine(res, cur_heavy_path_max);
    }
    if (depth[a] > depth[b])
        swap(a, b);
    int last_heavy_path_max = segment_tree_query(pos[a], pos[b]);
    res = combine(res, last_heavy_path_max);
    return res;
}

void decompose(int v, int h, vector<vector<int>> const& adj) {
    head[v] = h, pos[v] = cur_pos++;        // Now v is present at pos[v]
    if (heavy[v] != -1)
        decompose(heavy[v], h, adj);
    for (int c : adj[v]) {
        if (c != parent[v] && c != heavy[v])
            decompose(c, c, adj);
    }
}

void HLD_init(vector<vector<int>> const& adj) {
    int n = adj.size();
    parent = vector<int>(n);
    depth = vector<int>(n);
    heavy = vector<int>(n, -1);
    head = vector<int>(n);
    pos = vector<int>(n);
    cur_pos = 0;

    dfs(0, adj);
    decompose(0, 0, adj);
}

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

    HLD_init(adj);
    for (int i = 0;i < n;i++) {
        segtree_update(0, 0, segtree_size - 1, pos[i], v[i]);
    }

    for (int i = 0;i < q;i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int s, x;
            cin >> s >> x;
            s--;
            segtree_update(0, 0, segtree_size - 1, pos[s], x);
        }
        else {
            int s;
            cin >> s;
            s--;
            cout << query(0, s) << " ";
        }
    }
    cout << endl;
}