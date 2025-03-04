// Find a solution to ax + by = 1
int gcd(int a,int b,ll &x,ll &y){
	x=1; y=0;
	ll x1=0,y1=1;
	int a1=a,b1=b;
  	while(b1){
    	int q=a1/b1;
    	tie(x,x1)=make_tuple(x1,x-q*x1);
    	tie(y,y1)=make_tuple(y1,y-q*y1);
    	tie(a1,b1)=make_tuple(b1,a1-q*b1);
	}
	return a1;
}
bool find_any_solution(int a,int b,int c,ll &x0,ll &y0,int &g){
	g=gcd(a,b,x0,y0); // If +ve gcd is required use g=abs(g). But after calling find_any_solution() function. Not in the next line.
	if(!g){
		if(c) return false;
    	else{
			x0=y0=0;
			return true;
    	}
	}
	if(c%g) return false;
	x0 *= c/g;
	y0 *= c/g;
	return true;
}
// Count the number of soln:
// x in range [min_x,max_x], y in range [min_y,max_y]
ll find_all_solutions(int a,int b,int c,int min_x,int max_x,int min_y,int max_y){
    ll x,y;
	int g;
    if(min_x>max_x || min_y>max_y || (!find_any_solution(a, b, c, x, y, g))) return 0;
    if(!a && !b) return (max_x-min_x+1)1ll(max_y-min_y+1); // long long needed for this case only
    c/=g;
    if(!a) return (min_y<=c && c<=max_y)*(max_x-min_x+1);
    else if(!b) return (min_x<=c && c<=max_x)*(max_y-min_y+1);
    
	a/=g; b/=g; 
    int sign_a=a>0?1:-1, sign_b=b>0?1:-1;
    
	auto shift=[&](ll k){
		x+=k*b;
		y-=k*a;
	};
	
	
	shift((min_x-x)/b);
    if(x<min_x) shift(sign_b);
    if(x>max_x) return 0;
    ll lx1 = x;

    shift((max_x-x)/b);
    if(x>max_x) shift(-sign_b);
    ll rx1=x;

    shift(-(min_y-y)/a);
    if(y<min_y) shift(-sign_a);
    if(y>max_y) return 0;
    ll lx2=x;

    shift(-(max_y-y)/a);
    if(y>max_y) shift(sign_a);
    ll rx2=x;

    if(lx2>rx2) swap(lx2, rx2);
    ll lx=max(lx1,lx2),rx=min(rx1,rx2);

    if(lx>rx) return 0;
    return (rx-lx)/abs(b)+1;
}