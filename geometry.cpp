# include <iostream>
# include <cstdio>
# include <cstring>
# include <vector>
# include <queue>
# include <cmath>
using namespace std;

struct point {
    double x, y;
    point() {}
    point(double _x,double _y):x(_x), y(_y) {}
    point operator+(const point &c) const{ return point(x + c.x, y + c.y); }
    point operator-(const point &c) const{ return point(x - c.x, y - c.y); }
    double operator*(const point&c) const{ return x*c.y - y*c.x; }
    double dot(const point&c) const{ return x*c.x + y*c.y; }
    void read() { scanf("%lf%lf", &x, &y); }
};

const point O(0,0);
const double eps = 1e-9;
int sgn(const double x) {
    if(fabs(x) < eps) return 0;
    return x > 0 ? 1 : -1;
}

bool insect(const point&a,const point&b,const point&c,const point&d) {
    int cs1 = sgn((b-a)*(c-a)) * sgn((b-a)*(d-a));
    int cs2 = sgn((d-c)*(a-c)) * sgn((d-c)*(b-c));
    return ((cs1<0&&cs2<=0)||(cs1<=0&&cs2<0));
}

bool cross(vector<point> &g,point &p) {
    for(size_t i=1; i<g.size(); ++i)
        if(insect(O, p, g[i-1], g[i])) return true;
    if(insect(O, p, *g.rbegin(), g[0]))
        return true;
    return false;
}

