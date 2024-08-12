// CF Problem Link : https://codeforces.com/problemset/problem/193/A
// Submission Link : https://codeforces.com/contest/193/submission/275857083

#include<bits/stdc++.h>
using namespace std;

int n; 
vector<vector<int>> adj;
vector<bool> visited;
vector<int> tin, low;
vector<pair<int,int>> bridge;
vector<int> cutpoint;
int timer;

void IS_BRIDGE(int v,int to) {
     bridge.push_back({v,to});
}

void dfs_bridge(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    for (int to : adj[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs_bridge(to, v);
            low[v] = min(low[v], low[to]); 
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
            dfs_bridge(i);
    }
}

void IS_CUTPOINT(int v) {
    cutpoint.push_back(v);
}

void dfs_points(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children=0;
    for (int to : adj[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs_points(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] >= tin[v] && p!=-1)
                IS_CUTPOINT(v);
            ++children;
        }
    }
    if(p == -1 && children > 1)
        IS_CUTPOINT(v);
}

void find_cutpoints() {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs_points(i);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n1,m1;
    cin>>n1>>m1;
    n=n1*m1;
    adj.resize(n);

    string s[n1];
    for(int i=0;i<n1;i++){
        cin>>s[i];
    }

    bool chk=0;

    for(int i=0;i<n1;i++)
    {
        for(int j=0;j<m1;j++)
        {
            if(i>0 && s[i][j]==s[i-1][j] && s[i][j]=='#')
            {
                adj[i*m1+j].push_back((i-1)*m1+j);
                adj[(i-1)*m1+j].push_back(i*m1+j);
            }

            if(j>0 && s[i][j]==s[i][j-1] && s[i][j]=='#')
            {
                adj[i*m1+j].push_back(i*m1+j-1);
                adj[i*m1+j-1].push_back(i*m1+j);
            }
        }
    }

    for(int i=0;i<n;i++)
    {
        if(adj[i].size()>1)
        {
            chk=1;
            break;
        }
    }

    find_cutpoints();

    if(cutpoint.size())
        cout<<1;
    else if(chk)
        cout<<2;
    else
        cout<<-1;
}