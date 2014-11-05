# include <iostream>
# include <cstdio>
# include <complex>
# include <cmath>
# include <cstring>
using namespace std;
namespace FFT {
    const int MAXN = 65536 * 4;
    const double eps = 1e-6;
    const double pi = acos(-1.0);
    complex<double> A[MAXN],B[MAXN],C[MAXN],e[MAXN];
    static int N;
    void DFT(complex<double> A[], complex<double> B[], int S, int L, int dt){
        if (L == 1){
            B[S] = A[S];
            return ;
        }
        int lim = L >> 1, mid = S + lim;
        for (int i = 0; i < lim; i++){
            B[S + i] = A[S + (i << 1)];
            B[mid + i] = A[S + (i << 1) + 1];
        }
        DFT(B, A, S, lim, dt << 1);
        DFT(B, A, mid, lim, dt << 1);
        for (int i = 0; i < lim; i++){
            B[S + i] = A[S + i] + e[i * dt] * A[mid + i];
            B[mid + i] = A[S + i] - e[i * dt] * A[mid + i];
        }
    }
    void init(int a[],int b[],int m) {
        for(N = 1; N < m; N <<= 1);
        N = N << 1;
        memset(A, 0, sizeof(A));
        memset(B, 0, sizeof(B));
        memset(C, 0, sizeof(C));
        for(int i=0; i<m; i++) A[i] = a[i];
        for(int i=0; i<m; i++) B[i] = b[i];
        for (int i = 0; i <= N; i++)
            e[i] = complex<double>(cos(2 * pi * i / N), sin(2 * pi * i / N));
    }
    void getResult(int c[]) {
        DFT(A, C, 0, N, 1);
        memcpy(A, C, sizeof(C));
        DFT(B, C, 0, N, 1);
        memcpy(B, C, sizeof(C));
        for (int i = 0; i < N; i++)
            C[i] = A[i] * B[i];
        for (int i = 0; i <= N; i++)
            e[i] = complex<double>(cos( - 2 * pi * i / N), sin( - 2 * pi * i / N));
        DFT(C, A, 0, N, 1);
        for(int i=0; i<N; i++)
            c[i] = A[i].real() / N + eps;
    }
}
int c[6];
int main() {
    int a[] = {1, 1, 1};
    int b[] = {1, 1, 1};
    FFT::init(a, b, 3);
    FFT::getResult(c);
    cout << c[2] << endl;
}
