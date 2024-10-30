// Based on CSES Mail Delivery problem
// Problem: https://cses.fi/problemset/task/1691
 
#include <bits/stdc++.h>
using namespace std;
 
class EulerianCircuit{
    int n,e;
 
    vector<vector<int>> adj;
    vector<bool> visit;
    vector<int> edges;
    int get(int u){
        while(visit[adj[u].back()])adj[u].pop_back();
        return adj[u].back();
    }
public:
    EulerianCircuit(int _n):n(_n),e(0){
        adj.resize(n);
        for(int i=0;i<n;++i)
            adj[i].push_back(0);
        edges.resize(2);
        visit.resize(2);
    }
    void addEdge(int u,int v){
        ++e;
        adj[u].push_back(e<<1);
        edges.push_back(v);
        adj[v].push_back((e<<1)|1);
        edges.push_back(u);
        visit.push_back(false);
        visit.push_back(false);
    }
    int findFirst(){
        int u = 0;
        for(int i=0;i<n;++i){
            if(adj[i].size()==1)continue;
            u = i;
            if(!(adj[i].size()&1))break;
        }
        return u;
    }
    vector<int> eulerCircuit(int u){
        if(!e)return vector<int>();
        stack<int> st;
        vector<int> circuit;
        st.push(u);
        while(!st.empty()){
            int x = get(st.top());
            if(x){
                visit[x] = visit[x^1] = true;
                st.push(edges[x]);
            }
            else{
                circuit.push_back(st.top());
                st.pop();
            }
        }
        return circuit;
    }
    vector<int> eulerCircuit(){
        if(!e)return vector<int>();
        return eulerCircuit(findFirst());
    }
};
 
int main() {
    int n, m;
    cin >> n >> m;
 
    EulerianCircuit ec(n);
    vector<int> degree(n, 0);
 
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        a--; b--; // Convert to 0-based index
        ec.addEdge(a, b);
        degree[a]++;
        degree[b]++;
    }
 
    // Check for Eulerian Circuit conditions
    for (int i = 0; i < n; ++i) {
        if (degree[i] % 2 != 0) {
            cout << "IMPOSSIBLE" << endl;
            return 0;
        }
    }
 
    vector<int> circuit = ec.eulerCircuit(0);
    if (circuit.size() != m + 1) { 
        // Check if all edges are covered
        cout << "IMPOSSIBLE" << endl;
    } else {
        for (int i : circuit) {
            cout << (i+1) << " ";
        }
        cout << endl;
    }
 
    return 0;
}
