#include<bits/stdc++.h>
using namespace std;

// This program is a solution to https://codeforces.com/contest/166/problem/B .

struct pt {
    double x, y;
    pt(double x=0, double y=0): x(x),y(y) {}
    bool operator == (const pt &rhs) const {
        return (x == rhs.x && y == rhs.y);
    }
    bool operator < (const pt &rhs) const {
        return y < rhs.y || (y==rhs.y && x < rhs.x);
    }
    bool operator > (const pt &rhs) const {
        return y > rhs.y || (y==rhs.y && x > rhs.x);
    }
};

double area(const vector<pt> &poly) {
    int n = static_cast<int>(poly.size());
    double area = 0; 
    for(int i=0;i<n;++i) {
        area += poly[i].x * (poly[(i+1)%n].y - poly[(i+n-1)%n].y);
    }
    area = fabs(area)/2;
    return area ;
}

double area(const vector<pt> &poly) {
    int n = static_cast<int>(poly.size());
    double area = 0; 
    for(int i=0;i<n;++i) {
       /*  pt p = i ? poly[i-1] : poly.back(), q = poly[i];
         area += (p.x - q.x) * (p.y - q.y) ; */
        // Use above formula for simple polygon
        area += poly[i].x * (poly[(i+1)%n].y - poly[(i+n-1)%n].y);
    }
    area = fabs(area)/2;
    return area ;
}


int orientation(pt a, pt b, pt c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // clockwise
    if (v > 0) return +1; // counter-clockwise
    return 0;
}

bool cw(pt a, pt b, pt c, bool include_collinear=false) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}
bool collinear(pt a, pt b, pt c) { return orientation(a, b, c) == 0; }

// Returns points in clokwise order with a[0] as left lowermost point(Minimum point)
void convex_hull(vector<pt>& a, bool include_collinear = false) {
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.y, a.x) < make_pair(b.y, b.x);
    });
    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = orientation(p0, a, b);
        if (o == 0)
            return (p0.x-a.x)*(p0.x-a.x) + (p0.y-a.y)*(p0.y-a.y)
                < (p0.x-b.x)*(p0.x-b.x) + (p0.y-b.y)*(p0.y-b.y);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)a.size()-1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin()+i+1, a.end());
    }

    vector<pt> st;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size()-2], st.back(), a[i], include_collinear))
            st.pop_back();
        st.push_back(a[i]);
    }

    a = st;
}

// poly: Contains points of polygon in counter clockwise order, with poly[0] as lower
// leftmost point(minimum point) and top is the index of top rightmost point(maximum point). 
// Min/Max are defined by comparator operator defined for points.
// It returns 1: Point outside, 0: Point in boundary, -1: Point inside. 
int pointVsConvexPolygon(pt &point, vector<pt> &poly, int top) {
    if(point < poly[0] || point > poly[top]) return 1;
    int o = orientation(point, poly[top], poly[0]);
    if(o == 0) {
        if(point == poly[0] || point == poly[top]) return 0;
        return (top == 1 || top+1==poly.size()) ? 0 : -1;
    } else if(o < 0) {
        auto itLeft = upper_bound(poly.rbegin(), poly.rend() - top-1, point);
        return orientation((itLeft == poly.rbegin() ? poly[0]:itLeft[-1]), itLeft[0], point);
    } else {
        auto itRight = lower_bound(poly.begin()+1,poly.begin()+top,point);
        return orientation(point, itRight[0], itRight[-1]);
    }
}

// Returns square of distance between point a and b
long double dist2(pt a, pt b) {
    return (a.x-b.x)*1ll*(a.x-b.x) + (a.y-b.y)*1ll*(a.y-b.y);
}

// a and b represent direction: Returns 1 if direction is ccw, 0 is coolinear
// and -1 is direction is cw (clockwise).
int ccw(const pt &a, const pt&b) {
    long double ans = (long double)a.x*b.y - (long double)a.y*b.x;
    if(ans < 0) return -1;
    return (ans > 0);
}

// Maximum distance (squared) between given sets of points 
// in O(N) or O(N*log(N)) (first make them a convex polygon)
long double maxDist2(vector<pt> &poly) {
    int n = static_cast<int>(poly.size());
    long double res = 0;
    for(int i = 0, j = n < 2 ? 0 : 1; i < j; ++i) 
        for(;; j = (j+1)%n) {
            res = max(res, dist2(poly[i], poly[j]));
            pt dir1 = pt(poly[i+1].x-poly[i].x, poly[i+1].y-poly[i].y);
            pt dir2 = pt(poly[(j+1)%n].x-poly[j].x, poly[(j+1)%n].y-poly[j].y);
            if(ccw(dir1, dir2) <= 0) break;
        }
    return res; 
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL); 
    #ifndef ONLINE_JUDGE
        freopen("input.txt","r",stdin);
        freopen("output.txt","w",stdout);
    #endif
    int n; cin >> n;
    vector<pt> A(n);
    for(int i=0;i<n;++i) cin >> A[i].x >> A[i].y;
    int pos = min_element(A.begin(), A.end()) - A.begin();
    vector<pt> temp = A;
    for(int i=0;i<n;++i) A[i] = temp[(pos+n-i)%n];
    int top = max_element(A.begin(),A.end()) - A.begin();
    int m; cin >> m;
    vector<pt> B(m);
    for(int i=0;i<m;++i) cin >> B[i].x >> B[i].y;
    for(int i=0;i<m;++i)
        if(pointVsConvexPolygon(B[i],A,top)!=-1) {
            cout << "NO\n"; return 0; 
        }
    cout << "YES\n";
    return 0;
}
