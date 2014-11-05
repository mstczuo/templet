# include <iostream>
# include <cstdio>
# include <cstring>
# include <cmath>

using namespace std;

const int MAXN = 240;
const double EPS = 1e-8;

void Guass(double a[][MAXN],int n,double ans[]) {
    static bool v[MAXN];
    int r = 0;
    for(int i = 0; i < n; ++i) v[i] = true;
    for(int j = 0; j < n; ++j) {
        for(int i = 0; i < n; ++i)
            if(v[i] && fabs(a[i][j])>EPS)
            { r = i; break; }
        v[r] = false;
        for(int i = 0; i < n; ++i)
            if(r != i) {
                double rate = a[i][j] / a[r][j];
                for(int k = j; k <= n; ++k)
                    a[i][k] -= rate * a[r][k];
            }
    }
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(fabs(a[i][j]) > EPS)
                ans[j] = a[i][n] / a[i][j];
}

/* solve Pell Equation { x^2 - Dy^2 = 1 }
 * return the minumum solution
 * find the k_th minumum solution:
 *
 *      x_n = x_n-1 * x1 + D * y_n-1 * y1
 *      y_n = x_n-1 * y1 + y_n-1 * x1
 */

bool pell( int D, int& x, int& y ) {  
    int sqrtD = sqrt(D + 0.0);  
    if( sqrtD * sqrtD == D ) return false;  
    int c = sqrtD, q = D - c * c, a = (c + sqrtD) / q;  
    int step = 0;  
    int X[] = { 1, sqrtD };  
    int Y[] = { 0, 1 };  
    while( true ) {  
        X[step] = a * X[step^1] + X[step];  
        Y[step] = a * Y[step^1] + Y[step];  
        c = a * q - c;  
        q = (D - c * c) / q;  
        a = (c + sqrtD) / q;  
        step ^= 1;  
        if( c == sqrtD && q == 1 && step ) {  
            x = X[0], y = Y[0];  
            return true;  
        }  
    }  
}  
