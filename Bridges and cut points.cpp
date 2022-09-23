int n; 
vector<vector<int>> adj;
vector<bool> visited;
vector<int> tin, low;
vector<pii> bridge;
int timer;

void IS_BRIDGE(int v,int to) {
     bridge.pb({v,to});
}

void dfs(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    for (int to : adj[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            // check for articulation point using condition: (low[to] >= tin[v] && p!=-1). If p==-1 check if no. of children > 1. 
            if (low[to] > tin[v])
                IS_BRIDGE(v, to);
        }
    }
}

void find_bridges() {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs(i);
    }
}
