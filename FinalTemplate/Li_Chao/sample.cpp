#include<bits/stdc++.h>
using namespace std;
#ifndef ONLINE_JUDGE
#ifdef __linux__
#include "/media/gakshat468/New Volume/CP/algo/debug.h"
#elif _WIN32
#include "D:/CP/algo/debug.h"
#endif
#else
#define debug(...) 468
#endif


typedef long long ll;
struct line {
    ll m, c;
    ll operator()(ll x) { return m * x + c; }
};


class Lichao {
    vector<line> lc_tree;
public:
    Lichao(int N) {
        lc_tree = vector<line>(N << 1, { 0, -(ll)(1e18) });
    }

    void insert(int v, int l, int r, line cur) {
        if (l + 1 == r) {
            if (lc_tree[v](l) < cur(l)) lc_tree[v] = cur;
            return;
        }
        int mid = (l + r) >> 1;
        int z = (mid - l) << 1;
        if (lc_tree[v].m > cur.m) swap(lc_tree[v], cur);
        if (cur(mid) > lc_tree[v](mid)) {
            swap(lc_tree[v], cur);
            insert(v + 1, l, mid, cur);
        }
        else
            insert(v + z, mid, r, cur);
    }


    ll query(int v, int l, int r, int pt) {       // Finding maximum value of function at x = pt
        if (l + 1 == r) return lc_tree[v](pt);
        int mid = (l + r) >> 1;
        int z = (mid - l) << 1;
        if (pt <= mid) return max(lc_tree[v](pt), query(v + 1, l, mid, pt));
        else return max(lc_tree[v](pt), query(v + z, mid, r, pt));
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    vector<ll> a(n + 2), p(n + 2);
    ll initans = 0, ans = 0;
    for (int i = 1;i <= n;i++) {
        cin >> a[i];
        initans += a[i] * i;
        p[i] = p[i - 1] + a[i];
    }
    ans = initans;
    p[n + 1] = a[n + 1] + p[n];
    int M = n + 1;
    Lichao ch(M);

    for (int i = 1;i <= n;i++) {
        ch.insert(0, 0, M - 1, { a[i],-a[i] * i + p[i] });
    }
    for (int i = n;i >= 0;i--) {
        ans = max(ans, initans - p[i] + ch.query(0, 0, M - 1, i));
    }

    cout << ans << endl;
}

