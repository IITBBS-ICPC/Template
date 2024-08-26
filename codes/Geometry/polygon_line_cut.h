// https://anurudhp.github.io/blogs/2021/11/12/icpc-wf-2020-domes.html

template <class T>
bool isZero(T x) { return x == 0; }

const double EPS = 1e-9;
template <>
bool isZero(double x) { return fabs(x) < EPS; }

template <class T> struct Point {
  typedef Point P;
  T x, y;
  explicit Point(T x = 0, T y = 0) : x(x), y(y) {}

  bool operator<(P p) const { return tie(x, y) < tie(p.x, p.y); }
  bool operator==(P p) const { return isZero(x - p.x) && isZero(y - p.y); }

  P operator+(P p) const { return P(x + p.x, y + p.y); }
  P operator-(P p) const { return P(x - p.x, y - p.y); }
  P operator*(T d) const { return P(x * d, y * d); }
  P operator/(T d) const { return P(x / d, y / d); }

  T dot(P p) const { return x * p.x + y * p.y; }
  T cross(P p) const { return x * p.y - y * p.x; }
  T cross(P a, P b) const { return (a - *this).cross(b - *this); }
};

// Line Intersection
template <class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
  auto d = (e1 - s1).cross(e2 - s2);
  if (isZero(d)) // if parallel
    return {-(s1.cross(e1, s2) == 0), P(0, 0)};
  auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
  return {1, (s1 * p + e1 * q) / d};
}

// Polygon Cut
typedef Point<double> P;
vector<P> polygonCut(const vector<P> &poly, P s, P e) {
  if (poly.size() <= 2) return {};
  vector<P> res;
  for (size_t i = 0; i < poly.size(); i++) {
    P cur = poly[i], prev = i ? poly[i - 1] : poly.back();
    if (isZero(s.cross(e, cur))) {
      res.push_back(cur);
      continue;
    }
    bool side = s.cross(e, cur) < 0;
    if (side != (s.cross(e, prev) < 0))
      res.push_back(lineInter(s, e, cur, prev).second);
    if (side)
      res.push_back(cur);
  }
  return res;
}

// Polygon Area, returns twice the actual area, signed.
template <class T>
T polygonArea2(const vector<Point<T>> &v) {
  T a = v.back().cross(v[0]);
  for (size_t i = 0; i + 1 < v.size(); i++)
    a += v[i].cross(v[i + 1]);
  return a;
}