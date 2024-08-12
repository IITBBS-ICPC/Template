// Problem Link : https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=24&page=show_problem&problem=1045
// Submission Link not available

#include<bits/stdc++.h>
using namespace std;

// Find a solution to ax + by = 1
int gcd(long long a, long long b, long long& x, long long& y) {
    x = 1, y = 0;
    int x1 = 0, y1 = 1, a1 = a, b1 = b;
    while (b1) {
        int q = a1 / b1;
        tie(x, x1) = make_tuple(x1, x - q * x1);
        tie(y, y1) = make_tuple(y1, y - q * y1);
        tie(a1, b1) = make_tuple(b1, a1 - q * b1);
    }
    return a1;
}

// find any solution to ax + by = c (g will store their gcd). 
// Generalized x and y can be given by: x = x0 + r*lcm(a,b)  and y = y0 - r*lcm(a,b)
bool find_any_solution(long long a, long long b, long long c, long long &x0, long long &y0, long long &g) {
	g = gcd(abs(a), abs(b), x0, y0);
	if (c % g) {
		return false;
	}
	x0 *= c / g;
	y0 *= c / g;
	if (a < 0) x0 = -x0;
	if (b < 0) y0 = -y0;
	return true;
}

int main()
{
    int a,b;
    while(cin>>a>>b)
    {
        long long x,y,g;
        find_any_solution(a,b,__gcd(a,b),x,y,g);
        cout<<x<<" "<<y<<" "<<g<<endl;
    }
}