// Yosupo Problem Link : https://judge.yosupo.jp/problem/system_of_linear_equations
// Submission Link : https://judge.yosupo.jp/submission/234410

#include<bits/stdc++.h>
using namespace std;

#define ll long long

const ll INF = 2; // it doesn't actually have to be infinity or a big number
const ll MOD = 998244353;

ll binexp(ll a,ll b)
{
    ll res=1;
    while(b>0)
    {
        if(b%2)
        {
            res=(res*a)%MOD;
        }
        a=(a*a)%MOD;
        b>>=1;
    }
    return res;
}

ll modinv(ll a)
{
    return binexp(a,MOD-2);
}

ll gauss (vector < vector<ll> > a, vector<ll> & ans,vector<vector<ll>> &basis) {
    ll n = (ll) a.size();
    ll m = (ll) a[0].size() - 1;

    vector<ll> where (m, -1);
    for (ll col=0, row=0; col<m && row<n; ++col) {
        ll sel = row;
        for (ll i=row; i<n; ++i)
            if (a[i][col] > a[sel][col])
                sel = i;
        if (a[sel][col] == 0)
            continue;
        for (ll i=col; i<=m; ++i)
            swap (a[sel][i], a[row][i]);
        where[col] = row;

        ll inv = modinv(a[row][col]);
        for (ll i=0; i<n; ++i)
            if (i != row) {
                ll c = (a[i][col] * inv)%MOD;
                for (ll j=col; j<=m; ++j)
                {
                    a[i][j] = (a[i][j] - (a[row][j] * c)%MOD + MOD )%MOD;
                }
            }
        ++row;
    }

    ans.assign (m, 0);
    for (ll i=0; i<m; ++i)
        if (where[i] != -1)
            ans[i] = (a[where[i]][m] * modinv(a[where[i]][i]) )%MOD;
    for (ll i=0; i<n; ++i) {
        ll sum = 0;
        for (ll j=0; j<m; ++j)
            sum = (sum + ans[j] * a[i][j]) %MOD;
        if (sum != a[i][m])
            return 0;
    }

    // Calculate the basis for the null space
    for (ll i = 0; i < m; ++i) {
        if (where[i] == -1) {
            // Free variable found, create a basis vector for it
            vector<ll> basis_vec(m, 0);
            basis_vec[i] = 1;  // Set the free variable to 1
            for (ll j = 0; j < m; ++j) {
                if (where[j] != -1) {
                    basis_vec[j] = (MOD - a[where[j]][i] * modinv(a[where[j]][j]) % MOD) % MOD;
                }
            }
            basis.push_back(basis_vec);
        }
    }

    for (ll i=0; i<m; ++i)
        if (where[i] == -1)
            return INF;
    return 1;
}

void solve()
{
    ll n,m;
    cin>>n>>m;

    vector<vector<ll>> a(n,vector<ll>(m+1,0));
    for(ll i=0;i<n;i++)
    {
        for(ll j=0;j<m;j++)
        {
            cin>>a[i][j];
        }
    }

    for(ll i=0;i<n;i++)
    {
        cin>>a[i][m];
    }

    vector<ll> ans;
    vector<vector<ll>> basis;

    ll res = gauss(a,ans,basis);

    if(res==0)
        cout<<-1;
    else
    {
        cout<<basis.size()<<"\n";

        for(ll i=0;i<m;i++)
        {
            cout<<ans[i]<<" ";
        }

        cout<<"\n";

        for(ll i=0;i<basis.size();i++)
        {
            for(ll j=0;j<m;j++)
            {
                cout<<basis[i][j]<<" ";
            }

            cout<<"\n";
        }
    }

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