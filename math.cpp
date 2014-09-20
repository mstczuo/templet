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

double A[MAXN][MAXN], ans[MAXN];

int id[22][22];
int main() {
    int cnt = 0;
    for(int i = 0; i < 20; ++i) {
        for(int j = 0; j <= i; ++j)
            id[i][j] = cnt++;
    }
    id[20][19] = cnt + 2;
    double P;
    while(~scanf("%lf", &P)) {
        int cur, to;
        for(int i = 0; i < 20; ++i) {
            for(int j = 0; j < i; ++j) {
                cur = id[i][j];
                A[cur][cur] = 1;
                A[cur][cnt] = 1;
                to = id[i][max(0, j - 2)];
                A[cur][to] -= (1-P);
                to = id[i][j+1];
                A[cur][to] -= P;
            }
            cur = id[i][i];
            A[cur][cur] = 1;
            A[cur][cnt] = 1;
            to = id[i][max(0, i - 2)];
            A[cur][to] -= (1-P);
            to = id[i+1][i];
            A[cur][to] -= P;
        }
        Guass(A, cnt, ans);
        printf("%.6lf\n", ans[0]);
    }
    return 0;
}

