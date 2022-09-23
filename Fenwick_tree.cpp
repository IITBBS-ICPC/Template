int bit[n];     // n = size of array (for which fenwick tree is made) + 1

void add(int idx,int val) {                                // adds val to range [l,n)
    for(++idx;idx<n;idx+=idx&(-idx)) bit[idx]+=val;
}

void range_add(int l,int r,int val) {    // adds val to range [l,r] (0-based).
    add(l,val);
    add(r+1,-val);
}
 
int query(int idx) {        // returns value of a[idx], in 0 based array. 
    int ans = 0;
    for(++idx;idx;idx-=idx&(-idx)) ans += bit[idx];  // ++idx is done because fenwick tree is 1 based indexed
    return ans;
}
