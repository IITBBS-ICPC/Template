
#include<bits/stdc++.h>
using namespace std;
class HLD {
    vector<int> segtree, parent, depth, heavy, head, pos, maxdepth;
    int cur_pos;

    int combine(int lval, int rval) {
        return max(lval, rval);
    }

    void segtree_update(int v, int l, int r, int x, int val) {
        if (x < l || r < x)return;
        if (l == r) {
            segtree[v] = val;
            return;
        }
        int mid = (l + r) >> 1;
        int z = (mid + 1 - l) << 1;
        if (x <= mid)
            segtree_update(v + 1, l, mid, x, val);
        else
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

    // HLD Template
    // Taken from cp-algotithms


    int dfs(int v, vector<vector<int>> const& adj) {
        int size = 1;
        int max_c_size = 0;
        maxdepth[v] = depth[v];
        for (int c : adj[v]) {
            if (c != parent[v]) {
                parent[c] = v, depth[c] = depth[v] + 1;
                int c_size = dfs(c, adj);
                size += c_size;
                if (c_size > max_c_size) {
                    max_c_size = c_size,
                        heavy[v] = c, maxdepth[v] = maxdepth[c];
                }
            }
        }
        return size;
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

public:
    HLD(vector<vector<int>> const& adj) {
        int n = adj.size();
        parent = depth = head = pos = maxdepth = vector<int>(n);
        heavy = vector<int>(n, -1);
        segtree = vector<int>(n << 1);
        cur_pos = 0;

        dfs(0, adj);
        decompose(0, 0, adj);
    }

    int query(int a, int b) {
        int res = 0;
        for (; head[a] != head[b]; b = parent[head[b]]) {

            if (depth[head[a]] > depth[head[b]])
                swap(a, b);
            int cur_heavy_path_res = segtree_query(pos[head[b]] << 1, depth[head[b]], maxdepth[b], depth[head[b]], depth[b]);
            res = combine(res, cur_heavy_path_res);
        }

        if (depth[a] > depth[b])
            swap(a, b);
        int last_heavy_path_max = segtree_query(pos[head[b]] << 1, depth[head[b]], maxdepth[b], depth[a], depth[b]);
        res = combine(res, last_heavy_path_max);
        return res;
    }


    void update(int x, int val) { // 0 based indexing
        segtree_update(pos[head[x]] << 1, depth[head[x]], maxdepth[x], depth[x], val);
    }
};