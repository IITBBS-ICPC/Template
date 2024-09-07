// f(a,b,c,n) = \sigma ( (a*x + b)/c ) from x=0 to n in 
//O(logn) where value is floor of the value.
// a>=0,b>=0,c>0
ll FloorSumAPPositive(ll a, ll b, 
  ll c, ll n){
  if(!a) return (b / c) * (n + 1);
  if(a >= c or b >= c) return ( ( n * (n + 1) ) / 2) * (a / c) 
  + (n + 1) * (b / c) + FloorSumAPPositive(a % c, b % c, c, n);
  ll m = (a * n + b) / c;
  return m * n - FloorSumAPPositive(c, c - b - 1, a, m - 1);
}

// f(a,b,c,n) = \sigma ( (a*x + b)/c ) from x=0 to n in 
//O(logn) where value is floor of the value.
ll FloorSumAP(ll a,ll b,
  ll c, ll n){
    if(c<0){
      c = -c;
      a = -a;
      b = -b;
    }
    ll _qa = (a>=0)?(a/c):((a-c+1)/c);
    ll _qb = (b>=0)?(b/c):((b-c+1)/c);
    ll _a = a-c*_qa;
    ll _b = b-c*_qb;
    return _qa*((n*(n+1))/2)
            +_qb*(n+1)+FloorSumAPPositive(_a,_b,c,n);
}

// f(a,b,c,n) = \sigma ( (a*x + b)/c ) from x=l to r in 
//O(logn) where value is floor of the value.
ll FloorSumAP(ll a,ll b,
  ll c, ll l, ll r){
    return FloorSumAP(a,b+a*l,c,r-l);
}