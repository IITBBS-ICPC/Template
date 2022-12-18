#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
    #define eprintf(...) fprintf(stderr, _VA_ARGS_)
#else
    #define eprintf(...) 42
#endif

using ll = long long;
using ld = long double;
using uint = unsigned int;
template<typename T>
using pair2 = pair<T, T>;
using pii = pair<int, int>;
using pli = pair<ll, int>;
using pll = pair<ll, ll>;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second

const ld eps = 1e-8;
bool eq(ld x, ld y) {
	return fabsl(x - y) < eps;
}
bool ls(ld x, ld y) {
	return x < y && !eq(x, y);
}
bool lseq(ld x, ld y) {
	return x < y || eq(x, y);
}

ld readLD() {
	int x;
	scanf("%d", &x);
	return x;
}

struct Point {
	ld x, y, z;

	Point() : x(), y(), z() {}
	Point(ld _x, ld _y, ld _z) : x(_x), y(_y), z(_z) {}

	void scan() {
		x = readLD();
		y = readLD();
		z = readLD();
	}

	Point operator + (const Point &a) const {
		return Point(x + a.x, y + a.y, z + a.z);
	}
	Point operator - (const Point &a) const {
		return Point(x - a.x, y - a.y, z - a.z);
	}
	Point operator * (const ld &k) const {
		return Point(x * k, y * k, z * k);
	}
	Point operator / (const ld &k) const {
		return Point(x / k, y / k, z / k);
	}
	ld operator % (const Point &a) const {
		return x * a.x + y * a.y + z * a.z;
	}
	Point operator * (const Point &a) const {
		return Point(
			y * a.z - z * a.y,
			z * a.x - x * a.z,
			x * a.y - y * a.x
			);
	}
	ld sqrLen() const {
		return *this % *this;
	}
	ld len() const {
		return sqrtl(sqrLen());
	}
	Point norm() const {
		return *this / len();
	}
};

ld ANS;
//triangle contains 4 points, 4th point is a copy of 1st
Point A[4], B[4];

//P lies on plane, n is perpendicular, return A' such that AA' is parallel plane,
//PA' is perpendicular
Point getHToPlane(Point P, Point A, Point n) {
	n = n.norm();
	return P + n * ((A - P) % n);
}

//checks if point P is in traingle t
bool inTriang(Point P, Point* t) {
	ld S = 0;
	S += ((t[1] - t[0]) * (t[2] - t[0])).len();
	for (int i = 0; i < 3; i++)
		S -= ((t[i] - P) * (t[i + 1] - P)).len();
	return eq(S, 0);
}
//assume A,B,C are on same line finds if C is between B
bool onSegm(Point A, Point B, Point C) {
	return lseq((A - B) % (C - B), 0);
}
//A is a point on line, a is direction of line, B is point on plain,
//b is normal to plane, l is used to store result in case intersection exists
bool intersectLinePlane(Point A, Point a, Point B, Point b, Point &I) {
	if (eq(a % b, 0)) return false;
	ld t = ((B - A) % b) / (a % b);
	I = A + a * t;
	return true;
}
//A is point on line, a is direction of line
//returns projection of P on line
Point getHToLine(Point P, Point A, Point a) {
	a = a.norm();
	return A + a * ((P - A) % a);
}
//get minimum distance of A from PQ
ld getPointSegmDist(Point A, Point P, Point Q) {
	Point H = getHToLine(A, P, Q - P);
	if (onSegm(P, H, Q)) return (A - H).len();
	return min((A - P).len(), (A - Q).len());
}
//
ld getSegmDist(Point A, Point B, Point C, Point D) {
	ld res = min(min(getPointSegmDist(A, C, D), getPointSegmDist(B, C, D)),
				min(getPointSegmDist(C, A, B), getPointSegmDist(D, A, B)));
	Point n = (B - A) * (D - C);
	if (eq(n.len(), 0)) return res;
	n = n.norm();
	Point I1, I2;
	if (!intersectLinePlane(A, B - A, C, (D - C) * n, I1)) throw;
	if (!intersectLinePlane(C, D - C, A, (B - A) * n, I2)) throw;
	if (!onSegm(A, I1, B)) return res;
	if (!onSegm(C, I2, D)) return res;
	return min(res, (I1 - I2).len());
}


void solve() {
	ANS = 1e20;
	for (int i = 0; i < 3; i++)
		A[i].scan();
	for (int i = 0; i < 3; i++)
		B[i].scan();
	A[3] = A[0];
	B[3] = B[0];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ANS = min(ANS, getSegmDist(A[i], A[i + 1], B[j], B[j + 1]));
	Point nA = ((A[1] - A[0]) * (A[2] - A[0])).norm();
	Point nB = ((B[1] - B[0]) * (B[2] - B[0])).norm();
	for (int i = 0; i < 3; i++) {
		Point I;
		if (!intersectLinePlane(A[i], A[i + 1] - A[i], B[0], nB, I)) continue;
		if (onSegm(A[i], I, A[i + 1]) && inTriang(I, B)) {
			ANS = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		Point I;
		if (!intersectLinePlane(B[i], B[i + 1] - B[i], A[0], nA, I)) continue;
		if (onSegm(B[i], I, B[i + 1]) && inTriang(I, A)) {
			ANS = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		Point P = getHToPlane(A[i], B[0], nB);
		if (inTriang(P, B)) ANS = min(ANS, (A[i] - P).len());
	}
	for (int i = 0; i < 3; i++) {
		Point P = getHToPlane(B[i], A[0], nA);
		if (inTriang(P, A)) ANS = min(ANS, (B[i] - P).len());
	}
	printf("%.13lf\n", (double)ANS);
}

int main()
{
	int t;
	scanf("%d", &t);
	while(t--) solve();

    return 0;
}
