using ll = long long;
using ld = long double;
using uint = unsigned int;
template<typename T>
using pair2 = pair<T, T>;
using pii = pair<int, int>;
using pli = pair<ll, int>;
using pll = pair<ll, ll>;

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

// P lies on plane, n is perpendicular, return A' such
// that AA' is parallel plane,
// PA' is perpendicular
Point getHToPlane(Point P, Point A, Point n) {
	n = n.norm();
	return P + n * ((A - P) % n);
}

// Checks if point P is in traingle t
bool inTriang(Point P, Point* t) {
	ld S = 0;
	S += ((t[1] - t[0]) * (t[2] - t[0])).len();
	for (int i = 0; i < 3; i++)
		S -= ((t[i] - P) * (t[i + 1] - P)).len();
	return eq(S, 0);
}
// Assuming A,B,C are on same line,
// it finds if C is between B
bool onSegm(Point A, Point B, Point C) {
	return lseq((A - B) % (C - B), 0);
}
//A is a point on line, a is direction of line, B is
// point on plane,
//b is normal to plane, l is used to store result in
// case intersection exists
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