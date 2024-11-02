int gcd(int a,int b){
	if (!a || !b) return a|b;
  	unsigned shift=__builtin_ctzll(a|b);
  	a>>=__builtin_ctzll(a);
  	do{
    	b>>=__builtin_ctzll(b);
    	if(a>b) swap(a,b);
    	b-=a;
  	}while(b);
 	return a<<shift;
}
