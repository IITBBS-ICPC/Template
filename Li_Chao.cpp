// Important : Here range l,r denotes [l,r). With this a bug is avoided which is if we take [l,r] then when both l and r are negative the update [l,mid] can result in infinite 
//             recursion. E.g: l = -7, r = -6. mid = (l+r)/2 = -6 so [l,mid] = [-7,-6], resulting in infinite recursion.

const int N = 1000000;
struct line{
    ll m,c;
    ll operator()(ll x) { return m*x + c; }
}Lichao[5*N];

void init() {
    // Use default case m such that no input line contains that.
    for(int i=0;i<5*N;++i) Lichao[i].m = -1;
}

void insert(int v,int l, int r,line cur) {
    if(Lichao[v].m == -1) {  // No line in this region
        Lichao[v] = cur;
        return ;
    }
    if(l+1==r) {
        if(Lichao[v](l) < cur(l)) Lichao[v] = cur;
        return ;
    }
    int mid = (l+r)/2; 
    if(Lichao[v].m > cur.m) swap(Lichao[v] , cur);
    if(cur(mid) > Lichao[v](mid)) {
        swap(Lichao[v] , cur);
        insert(v<<1, l, mid, cur);
    } else
        insert((v<<1)|1, mid, r, cur);
}


ll query(int v, int l,int r, int pt) {       // Finding maximum value of function at x = pt
    if(Lichao[v].m == -1) return -1e18;      // No line in this region
    if(l+1==r) return Lichao[v](pt);
    int mid = (l+r)/2;
    if(pt <= mid) return max(Lichao[v](pt), query(v<<1, l, mid, pt));
    else if(pt > mid) return max(Lichao[v](pt), query((v<<1)|1, mid, r, pt));
}
