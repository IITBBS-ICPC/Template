// Codeforces Submission
// Problem Link: https://codeforces.com/problemset/problem/508/D
// Submission Link: https://codeforces.com/contest/508/submission/275853875

#include <bits/stdc++.h>
using namespace std;

class DirectedEulerianCircuit{
    int n;
    vector<stack<int>> adj;
    vector<int> deg;

    int get(int u){
        if(adj[u].empty())
            return -1;
        return adj[u].top();
    }
public:
    DirectedEulerianCircuit(int _n):n(_n){
        adj.assign(n,stack<int>());
        deg.assign(n,0);
    }
    void addEdge(int u,int v){
        adj[u].push(v);
        deg[u]++;
        deg[v]--;
    }
    int findFirst(){
        for(int i=0;i<n;++i){
            if(deg[i]==1)
                return i;
        }
        return 0;
    }
    vector<int> eulerCircuit(int u = -1){
        stack<int> st;
        if(u==-1)
            u=findFirst();
        vector<int> circuit;
        st.push(u);
        while(!st.empty()){
            int x = get(st.top());
            if(x!=-1){
                adj[st.top()].pop();
                st.push(x);
            }
            else{
                circuit.push_back(st.top());
                st.pop();
            }
        }
        reverse(circuit.begin(),circuit.end());
        return circuit;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin>>n;

    unordered_map<string,int> nodes;
    string revNodes[n+1];
    int largestNode=0;

    DirectedEulerianCircuit ec(n+1);

    for(int i=0;i<n;++i){
        string names;
        cin>>names;
        string a=names.substr(0,2);
        string b=names.substr(1,2);

        if(nodes.find(a)==nodes.end())
            nodes[a]=largestNode++;
        if(largestNode>n+1)
        {
            cout<<"NO\n";
            return 0;
        }
        if(nodes.find(b)==nodes.end())
            nodes[b]=largestNode++;
        if(largestNode>n+1)
        {
            cout<<"NO\n";
            return 0;
        }
        revNodes[nodes[a]]=a;
        revNodes[nodes[b]]=b;
        ec.addEdge(nodes[a],nodes[b]);
    }

    vector<int> circuit=ec.eulerCircuit();

    for(int i=0;i<circuit.size()-1;++i){
        if(revNodes[circuit[i]][1]!=revNodes[circuit[i+1]][0]){
            cout<<"NO\n";
            return 0;
        }
    }

    if(circuit.size()!=n+1){
        cout<<"NO\n";
        return 0;
    }

    cout<<"YES\n";

    for(int i=0;i<circuit.size()-1;++i)
        cout<<revNodes[circuit[i]][0];

    cout<<revNodes[circuit.back()]<<'\n';

    return 0;
}
