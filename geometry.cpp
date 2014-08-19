# include <iostream>
# include <cstdio>
# include <cstring>
# include <vector>
# include <queue>
# include <cmath>
using namespace std;

const double eps = 1e-9;
int sgn(const double x) {
    if(fabs(x) < eps) return 0;
    return x > 0 ? 1 : -1;
}

struct Point {
    double x, y;
    Point() {}
    Point(double _x,double _y):x(_x), y(_y) {}
    Point operator+(const point &c) const{ return point(x + c.x, y + c.y); }
    Point operator-(const point &c) const{ return point(x - c.x, y - c.y); }
    Point operator*(const double&c) const{ return point(x * c, y * c); }
    Point operator/(const double&c) const{ return point(x/c, y/c); }
    double operator*(const Point&c) const{ return x*c.y - y*c.x; }
    double dot(const Point&c) const{ return x*c.x + y*c.y; }
    double norm() { return sqrt(x * x + y * y; }
    void read() { scanf("%lf%lf", &x, &y); }
};

const Point O(0,0);

bool insect(const Point&a,const point&b,const point&c,const point&d) {
    int cs1 = sgn((b-a)*(c-a)) * sgn((b-a)*(d-a));
    int cs2 = sgn((d-c)*(a-c)) * sgn((d-c)*(b-c));
    return ((cs1<0&&cs2<=0)||(cs1<=0&&cs2<0));
}

bool cross(vector<Point> &g,point &p) {
    for(size_t i=1; i<g.size(); ++i)
        if(insect(O, p, g[i-1], g[i])) return true;
    if(insect(O, p, *g.rbegin(), g[0]))
        return true;
    return false;
}

double Angle(const Point &a,const point &b) {
    return acos(dot(a,b)) / a.norm() / b.norm();
}

Point rotate(const point&a,double rad) {
    return Point(a.x*cos(rad)-a.y*sin(rad), a.x*sin(rad)+a.y*cos(rad));
}

Point Intersection(Point P,Point Q,Point A,Point B) {
    Point u = P - A;
    double t = ((B-A) * u) / ((Q-P)*(B-A));
    return P + (Q-P) * t;
}

Point InterSection2(Point P,Vector v,Point Q,Vector w) {
    Vector u = P - Q;
    double t = (w * u)/(v * w);
    return P + v * t;
}

double DistanceToLine(Point P,Point A,Point B) {
    Point v1 = B - A, v2 = P - A;
    return fabs(v1 * v2 / v1.nrom());
}

double DistanceToSegment(Point P,Point A,Point B) {
    if(A == B) return (P - A).norm();
    Point v1 = B - A, v2 = P - A, v3 = P - B;
    if(sgn(v1 * v2) < 0) return v2.norm();
    if(sgn(v1 * v3) > 0) return v3.norm();
    return fabs(v1 * v2 / v1.norm());
}

Point GetLineProjection(Point P,Point A,Point B) {
    vector v = B - A;
    return A + v * ((v * (P - A)) / (v * v));
}

double PolygonArea(Point *P,int n) {
    double area = 0;
    for(int i=1; i<n-1; ++i)
        area += (p[i]-p[0])*(p[i+1]-p[0]);
    return area/2;
}


