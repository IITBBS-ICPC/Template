typedef long double ld;
typedef vector<ld> vld;
typedef pair<ld, ld> pld;

#define x first
#define y second
#define opr operator
istream &opr >> (istream & in, pld &p) {
  in >> p.x >> p.y;
  return in;
}
ostream &opr << (ostream & out, pld &p) {
  out << "(" << p.x << "," << p.y << ")";
  return out;
}
pld opr - (pld & p, pld &q) {
  return {p.x - q.x, p.y - q.y};
}
pld opr + (pld & p, pld &q) {
  return {p.x + q.x, p.y + q.y};
}
ld opr *(const pld &p, const pld &q) {
  return p.x * q.x + p.y * q.y;
} // Dot product
ld opr
  ^ (const pld &p,
     const pld
       &q) { return p.x * q.y - p.y * q.x; } // Cross
                                             // Product

  const ld PI
  = 3.14159265359;
const ld eps = 1e-10;

ld mod(pld p) { return sqrtl(p * p); }
ld Point_Angle(pld p) {
  ld ans = atan2(p.y, p.x);
  if(ans < 0) ans += 2 * PI;
  return ans;
}
ld Vector_Angle(pld p1, pld p2) {
  // Vector is from 1 -> 2
  return Point_Angle(p2 - p1);
}
ld Angle_between_Vectors(pld p1, pld p2, pld p3,
                         pld p4) {
  // Vectors are a:1 -> 2 and b:3 -> 4. Angle of b w.r.t
  // a in anti-clockwise direction [0,360)
  ld ans = Vector_Angle(p3, p4) - Vector_Angle(p1, p2);
  if(ans < 0) ans += 2 * PI;
  return ans;
}
ld Angle_between_Vectors2(pld p1, pld p2, pld p3,
                          pld p4) {
  // Vectors are 1 -> 2 and 3 -> 4. Only [0,180] angles
  // are considered (magnitude) [Dot Product]
  p2 = p2 - p1;
  p4 = p4 - p3;
  ld ans = acos((p2 * p4) / (eps + mod(p2) * mod(p4)));
  return ans;
}
vector<ld> Bisector(pld x, pld y, pld z) {
  // Bisector of XY and XZ. Returns ax+by+c=0;
  ld angle1 = Vector_Angle(x, y), angle2
                                  = Vector_Angle(x, z);
  ld angle = (angle1 + angle2) / 2;
  ld a, b, c;
  if(abs(angle - PI / 2) <= eps
     || abs(angle - 3 * PI / 2) <= eps) {
    a = 1;
    b = 0;
  } else {
    a = -tan(angle);
    b = 1;
  }
  c = -a * x.x - b * x.y;
  return {a, b, c};
}
vld Equation_of_line(pld p1, pld p2) {
  return {p2.y - p1.y, p1.x - p2.x, (p2 ^ p1)};
}
pld Point_of_Intersection(pld a, pld b, pld c) {
  // Normal lines. pair "a" represent {a1,a2} of both
  // lines. Assuming they intersect
  return {(ld)(b ^ c) / (a ^ b), (ld)(c ^ a) / (a ^ b)};
}
pld Base_of_altitude(pld a, pld b, pld c) {
  // Point of intersection of Altitude from Point a to
  // line (b-c) [base]
  vld bc = Equation_of_line(b, c);
  vld alt_A
    = {bc[1], -bc[0], bc[0] * a.y - bc[1] * a.x};
  pld pt = Point_of_Intersection({bc[0], alt_A[0]},
                                 {bc[1], alt_A[1]},
                                 {bc[2], alt_A[2]});
  return pt;
}
ld Distance_of_point_to_Line(pld p, pld ab, ld c) {
  return abs(ab * p + c) / mod(ab);
}
ld Distance_of_point_to_Line3(pld p, pld p1, pld p2) {
  // Line has p1 and p2 as end points
  return abs((p ^ (p2 - p1)) + (p2 ^ p1))
         / mod(p2 - p1);
}
ld Distance_of_point_to_Ray(pld p, pld p1, pld p2) {
  // Ray p1 -> p2
  ld ans;
  if(((p - p1) * (p2 - p1)) > 0)
    ans = Distance_of_point_to_Line3(p, p1, p2);
  else
    ans = mod(p - p1);
  return ans;
}
ld Distance_of_point_to_Line_Segment(pld p, pld p1,
                                     pld p2) {
  ld ans;
  if(((p - p1) * (p2 - p1)) > 0
     && (p - p2) * (p1 - p2) > 0)
    ans = Distance_of_point_to_Line3(p, p1, p2);
  else
    ans = min(mod(p - p1), mod(p - p2));
  return ans;
}
ld Distance_between_Line_Segments(pld p1, pld p2,
                                  pld p3, pld p4) {
  // min dist , v1:1->2 and v2:3->4
  ld ans;
#define LS Distance_of_point_to_Line_Segment
  if(((p2 - p1) ^ (p3 - p1)) * ((p2 - p1) ^ (p4 - p1))
       < 0
     && ((p4 - p3) ^ (p1 - p3))
            * ((p4 - p3) ^ (p2 - p3))
          < 0)
    ans = 0;
  else
    ans = min({LS(p1, p3, p4), LS(p2, p3, p4),
               LS(p3, p1, p2), LS(p4, p1, p2)});
#define LS LS
  return ans;
}
ld Distance_between_Rays(pld p1, pld p2, pld p3,
                         pld p4) {
#define RD Distance_of_point_to_Ray
#define AV Angle_between_Vectors2
  ld ans = min(RD(p1, p3, p4), RD(p3, p1, p2));
  if(AV(p1, p2, p4, p3) > AV(p1, p2, p1, p3)
     && AV(p3, p4, p2, p1) > AV(p3, p4, p3, p1))
    return 0;
#define RD RD
#define AV AV
  return ans;
}
