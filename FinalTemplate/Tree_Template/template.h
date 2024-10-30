#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#include<bits/stdc++.h>
using namespace std;

#define pb push_back
#define mp make_pair
#define F first
#define S second
#define lb lower_bound
#define ub upper_bound
#define sz(x) (int)(x).size()
#define all(x) x.begin(), x.end()

typedef long long ll;
typedef long double ld;
typedef complex<ld> cd;
typedef pair<int, int> pii;
typedef pair<ll,ll> pll;
typedef pair<ld,ld> pld;
typedef vector<int> vi;
typedef vector<ld> vd;
typedef vector<ll> vl;
typedef vector<pii> vpi;
typedef vector<pll> vpl;

#define FOR(i, a, b) for (int i=a; i<(b); i++)
#define F0R(i, a) for (int i=0; i<(a); i++)
#define FORd(i,a,b) for (int i = (b)-1; i >= a; i--)
#define F0Rd(i,a) for (int i = (a)-1; i >= 0; i--)
#define trav(a,x) for (auto& a : x)

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll powmod(ll a,ll b) {ll res=1; assert(b>=0); for(;b;b>>=1){if(b&1)res=res*a;a=a*a;}return res;}
ll gcd(ll a,ll b) { return b?gcd(b,a%b):a;}

class Tree {
public:
    bool useEuler;
    int n, l;
    vector<vector<int>> g;
    int timer;
    vector<int> tin, tout, dep, euler, lg;
    // vector<int> lg(k+1);
    vector<vector<int>> up, euler_sparse; 
    // up[i][j] stores 2^j th ancestor of i, euler_sparse is sparse table used to calculate lca in O(1)

    Tree(int sz): n(sz) {g.resize(sz);}

    void dfs(int v, int p)
    {
        dep[v] = dep[p] + 1;
        tin[v] = ++timer;
        up[v][0] = p;
        for (int i = 1; i <= l; ++i)
            up[v][i] = up[up[v][i-1]][i-1];

        for (int u : g[v]) {
            if (u != p)
                dfs(u, v);
        }

        tout[v] = ++timer;
    }

    bool is_ancestor(int u, int v)
    {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }

    int binary_lca(int u, int v)
    {
        if (is_ancestor(u, v))
            return u;
        if (is_ancestor(v, u))
            return v;
        for (int i = l; i >= 0; --i) {
            if (!is_ancestor(up[u][i], v))
                u = up[u][i];
        }
        return up[u][0];
    }

    int dist(int a,int b) {
        return dep[a] + dep[b] - 2*dep[lca(a,b)];
    }

    void euler_dfs(int u,int p) {
        dep[u] = dep[p] + 1;
        tin[u] = euler.size();
        euler.pb(u);
        for(auto v:g[u]) {
            if(v!=p) {
                dep[v] = dep[u]+1;
                euler_dfs(v,u);
                euler.pb(u);
            }
        }
        tout[u] = euler.size()-1;
    }

    void sparse() {
        for(int i=2;i<=euler.size();++i) lg[i]=lg[i>>1]+1;
        for(int i=0;i<euler.size();++i) euler_sparse[i][0] = euler[i];
        for(int i=1;i<=l;++i) {
            for(int j=0;(j+(1<<i)-1)<euler.size();++j)
            {
                int u = euler_sparse[j][i-1];
                int v = euler_sparse[j+(1<<(i-1))][i-1];
                euler_sparse[j][i] = (dep[u]<dep[v] ? u:v);
            }
        }
    }

    int euler_lca(int u,int v) {
        if(tin[u]<tin[v]) swap(u,v);
        int len = tin[u] - tin[v] + 1;
        int k = lg[len];
        int l1 = euler_sparse[tin[v]][k],l2 = euler_sparse[tin[u]-(1<<k)+1][k];
        return dep[l1]<dep[l2]?l1:l2;
    }

    int lca(int u,int v) {
        if(useEuler) return euler_lca(u,v);
        return binary_lca(u,v);
    }

    void preprocess(int root, bool iseuler = 0) {
        tin.resize(n);
        tout.resize(n);
        dep.resize(n, 0);
        timer = 0; useEuler = iseuler;
        if(iseuler) {
            l = ceil(log2(2*n));
            euler_dfs(root, root);
            lg.resize(euler.size()+1, 0);
            euler_sparse.assign(euler.size()+1, vector<int>(l+1));
            sparse();
        } else {
            l = ceil(log2(n));
            up.assign(n, vector<int>(l + 1));
            dfs(root, root);
        }
    }
  
    
    // This function is not verified, others are verified.
    int AuxillaryTree(vector<int> &v, vector<vector<pii>> &g)
    {
        sort(all(v),[&](int x,int y)
        {
            return tin[x]<tin[y];
        });
        int nx = sz(v);
        for(int i=0;i<nx-1;++i)
            v.push_back(lca(v[i],v[i+1]));
        sort(all(v),[&](int x,int y)
        {
            return tin[x]<tin[y];
        });
        v.resize(unique(all(v))-v.begin());
        vector<int> ed;
        nx = sz(v);
        ed.pb(v[0]);
        for(int i=1;i<nx;++i)
        {
            while(!is_ancestor(ed.back(),v[i]))
                ed.pop_back();
            int p = dist(v[i], ed.back());
            g[ed.back()].pb({v[i], p});
            g[v[i]].pb({ed.back(), p});
            ed.pb(v[i]);
        }
        return ed[0]; // return root.
    }
};

void solve() {
    int n, q; 
    cin >> n >> q;
    Tree T(n);
    for(int i=1;i<n;++i) {
        int x; cin >> x;
        T.g[x].pb(i);
        T.g[i].pb(x);
    }
    T.preprocess(0, 0); // First argument is root, second is if we should use euler lca or not(use binary lifting).

    while(q--) {
        int u, v; 
        cin >> u >> v;
        cout << T.lca(u, v) << '\n';
    }
}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL); 
    solve();
  return 0;
}