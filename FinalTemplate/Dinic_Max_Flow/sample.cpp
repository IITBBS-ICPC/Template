// Codeforces Problem Link : https://codeforces.com/gym/104871/problem/C
// Submission Link : https://codeforces.com/gym/104871/submission/275865531

#include<bits/stdc++.h>
using namespace std;

#define ll long long

/*
Implementation of Dinic's blocking algorithm
for the maximum flow.
Complexity: V^2 E (faster on real graphs).

please add edges not related to input first 
to improve constants

This class accepts a graph 
(costructed calling AddEdge) and then solves
the maximum flow problem for any source and sink. 

Both directed and undirected graphs are supported.
In case of undirected graphs, 
each edge must be added twice.

To compute the maximum flow just call 
GetMaxFlowValue(source, sink).
*/

template <typename T>
struct Dinic {
    struct Edge {
        int u, v;
        T cap, flow;
        Edge() {}
        Edge(int u, int v, T cap): u(u), v(v), cap(cap), flow(0) {}
    };

    int N;
    vector<Edge> edges; // The "inverse" edge of edges[i] is edges[i^1].
    vector<vector<int>> aa; // Stores the index of the edge in the edges vector.
    // dist is the distance in the bfs.
    // pt is used internally to save time in the dfs.
    vector<int> dist, pt;

    Dinic(int N): N(N), edges(0), aa(N), dist(N), pt(N) {}

    void AddEdge(int u, int v, T cap) {
        assert(0 <= u and u < N);
        assert(0 <= v and v < N);
        // dbg(u, v, cap);
        if (u != v) {
            edges.push_back(Edge(u, v, cap));
            aa[u].push_back(edges.size() - 1);
            // The inverse edge has 0 capacity.
            edges.push_back(Edge(v, u, 0));
            aa[v].push_back(edges.size() - 1);
        }
    }

    // Computes all distances from source and stores them in dist.
    // It returns true if sink is reachable from source.
    bool BFS(int source, int sink) {
        queue<int> q({source});
        fill(dist.begin(), dist.end(), N + 1);
        dist[source] = 0;
        while(!q.empty()) {
            int u = q.front(); q.pop();
            if (u == sink) break;
            for (int k : aa[u]) {
                Edge &e = edges[k];
                if (e.flow < e.cap && dist[e.v] > dist[e.u] + 1) {
                    dist[e.v] = dist[e.u] + 1;
                    q.push(e.v);
                }
            }
        }
        return dist[sink] != N + 1;
    }

    T DFS(int u, int sink, T flow = -1) {
        if (u == sink || flow == 0) return flow;
        // ACHTUNG: Be careful of using references (&) where needed!
        for (int &i = pt[u]; i < (int)aa[u].size(); i++) {
            Edge &e = edges[aa[u][i]];
            Edge &oe = edges[aa[u][i] ^ 1];
            if (dist[e.v] == dist[e.u] + 1) {
                T amt = e.cap - e.flow;
                if (flow != -1 && amt > flow) amt = flow;
                if (T pushed = DFS(e.v, sink, amt)) {
                    e.flow += pushed;
                    oe.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    T GetMaxFlowValue(int source, int sink) {
        for (Edge& e : edges) e.flow = 0;
        T res = 0;
        while (BFS(source, sink)) {
            fill(pt.begin(), pt.end(), 0);
            while (T flow = DFS(source, sink)) res += flow;
        }
        return res;
    }
};

void solve()
{
    ll g,c,t;
    cin>>g>>c>>t;

    ll src=0;
    ll sink=1;

    Dinic<ll> d(2+c+t);

    ll ingredientcost[g];
    ll cakecost[c];
    ll toolcost[t];

    for(ll i=0;i<c;i++)
    {
        cin>>cakecost[i];
    }

    for(ll i=0;i<g;i++)
    {
        cin>>ingredientcost[i];
    }

    for(ll i=0;i<t;i++)
    {
        cin>>toolcost[i];
    }

    for(ll i=0;i<c;i++)
    {
        for(ll j=0;j<g;j++)
        {
            ll x;
            cin>>x;

            cakecost[i]-=x*ingredientcost[j];
        }

        cakecost[i]=max(cakecost[i],0ll);
    }

    //

    ll ans=0;

    for(ll i=0;i<c;i++)
    {
        ans+=cakecost[i];
    }

    //

    for(ll i=2;i<=c+1;i++)
    {
        d.AddEdge(src,i,cakecost[i-2]);
    }

    for(ll i=c+2;i<=c+t+1;i++)
    {
        d.AddEdge(i,sink,toolcost[i-c-2]);
    }

    //

    for(ll i=0;i<c;i++)
    {
        ll num;
        cin>>num;

        for(ll j=0;j<num;j++)
        {
            ll x;
            cin>>x;

            d.AddEdge(i+2,c+x+1,(long long)1e9);
        }
    }

    cout<<ans-d.GetMaxFlowValue(src,sink);

    cout<<"\n";
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll t=1;
    // cin>>t;

    while(t--)
    {
        solve();
    }

    return 0;
}