# include <complex>
# include <cmath>

using namespace std;

typedef complex<double> Point;

double Dot(Point A,Point B) { return real(conj(A)*B); }
double Cross(Point A,Point B) { return imag(Conj(A)*B); }
Point Rotate(Point A,Point B) { return A*exp(Point(0, rad)); }

