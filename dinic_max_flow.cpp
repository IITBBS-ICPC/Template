#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Implementation of Dinic's blocking algorithm for the maximum flow.
// Complexity: V²E (faster on real graphs).

// please add edges not related to input first to improve constants

// This class accepts a graph (costructed calling AddEdge) and then solves
// the maximum flow problem for any source and sink. Both directed and
// undirected graphs are supported. In case of undirected graphs, each edge
// must be added twice.
//
// To compute the maximum flow just call GetMaxFlowValue(source, sink).
// If you are also interested in the actual flow values, just look at the
// flow variable in each edge with positive capacity. The edges are stored
// in a vector (named edges) and those with null capacity are the residual
// graph edges.
//
// If you are interested in the minimum cut, just read the dist vector.
// The vertices with dist < N+1 are those on the same side (with respect to
// the cut) of the source.

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

// Solution given for https://www.spoj.com/problems/FASTFLOW/ in the main function.

int main()
{
  int N, E;
  scanf("%d%d", &N, &E);
  Dinic<ll> dinic(N);
  for(int i = 0; i < E; i++)
  {
    int u, v;
    ll cap;
    scanf("%d%d%lld", &u, &v, &cap);
    dinic.AddEdge(u - 1, v - 1, cap);
    dinic.AddEdge(v - 1, u - 1, cap);
  }
  printf("%lld\n", dinic.GetMaxFlowValue(0, N - 1));
  return 0;
}