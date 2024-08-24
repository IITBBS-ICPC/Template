// Important : Here range l,r denotes [l,r). With this a bug is avoided which is if we take [l,r] then when both l and r are negative the update [l,mid] can result in infinite 
//             recursion. E.g: l = -7, r = -6. mid = (l+r)/2 = -6 so [l,mid] = [-7,-6], resulting in infinite recursion.

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
