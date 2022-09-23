int n;
vector<vector<pii>> tree,adj; // tree contains minimum spanning tree and adj contains graph in the form of {nextnode,cost} 
vector<int> parent,siz;  // Used to implement DSU

int find(int x){
    if(parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

bool same(int x, int y){
    return find(x) == find(y);
}

void unite(int x, int y){
    int a = find(x),b = find(y);
    if(a == b) return;
    if(siz[b] > siz[a]) swap(a,b);
    parent[b] = a;
    siz[a] += siz[b];
}

struct Edge {
    int u, v, weight;
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }
};

int kruskal(){
    int cost = 0;
    parent.resize(n);
    siz.resize(n);
    
    for(int i=0;i<n;++i){
        parent[i] = i;
        siz[i] = 1;
    }
    vector<Edge> edges;
    for(int i=0;i<n;++i){
        for(int j=0;j<adj[i].size();++j){
            if(adj[i][j].F > i){
                Edge temp = {i,adj[i][j].F,adj[i][j].S};
                edges.pb(temp);
            }
        }
    }
    sort(edges.begin(), edges.end());
    int cnt_nodes=0;
    for (Edge e : edges) {
        if (find(e.u) != find(e.v)) {
            cnt_nodes++;
            cost += e.weight;
            tree[e.u].pb({e.v,e.weight});
            tree[e.v].pb({e.u,e.weight});
            unite(e.u, e.v);
        }
        if(cnt_nodes == n-1 ) break;
    }
    return cost;
}
