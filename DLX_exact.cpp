#include <iostream> 
#include <cstring>
#include <cstdio>

using namespace std;
typedef long long ll;

template<int N,int M>
struct DLX {
    int n, m, size;
    int U[M], D[M], L[M], R[M], Row[M], Col[M];
    int H[M], S[M];
    int ansd, ans[N];
    int Limit;
    void init(int _n,int _m,int lim) {
        n = _n; m = _m; Limit = lim;
        for(int i = 0; i <= m; ++i) {
            S[i] = 0;
            U[i] = D[i] = i;
            L[i] = i - 1;
            R[i] = i + 1;
        }
        R[m] = 0; L[0] = m;
        size = m;
        for(int i = 1; i <= n; ++i) 
            H[i] = -1;
        ansd = 0;
    }
    void add(int r,int c) {
        ++S[Col[++size]=c];
        Row[size]=r;
        D[size]=D[c];
        U[D[c]]=size;
        U[size]=c;
        D[c]=size;
        if(H[r]<0)H[r]=L[size]=R[size]=size;
        else {
            R[size]=R[H[r]];
            L[R[H[r]]]=size;
            R[H[r]]=size;
            L[size]=H[r];
        }
    }
    void remove(int c) {
        L[R[c]]=L[c],R[L[c]]=R[c];
        for(int i = D[c]; i!=c; i=D[i]) {
            for(int j = R[i]; j!=i; j=R[j])
                U[D[j]]=U[j],D[U[j]]=D[j],--S[Col[j]];
        }
    }
    void resume(int c) {
        for(int i = U[c]; i!=c; i=U[i]) {
            for(int j = R[i]; j!=i; j=R[j])
                U[D[j]]=D[U[j]]=j,++S[Col[j]];
        }
        L[R[c]]=R[L[c]]=c;
    }
    void select(int r) {
        for(int i = R[H[r]]; i != H[r]; i = R[i]) 
            remove(Col[i]);
        remove(Col[H[r]]);
        ans[ansd++] = r;
    }
    bool main() {
        return Dance(ansd);
    }
    bool Dance(int d) {
        if(d > Limit) return false;
        if(R[0] == 0) { ansd = d; return true; }
        int c = R[0];
        for(int i = R[0]; i != 0;i = R[i])
            if(S[i] < S[c]) c = i;
        remove(c);
        for(int i = D[c]; i != c; i = D[i]) {
            for(int j = R[i]; j!=i; j=R[j]) remove(Col[j]);
            ans[d] = Row[i];
            if(Dance(d + 1)) return true;
            for(int j = L[i]; j!=i; j=L[j]) resume(Col[j]);
        }
        resume(c);
        return false;
    }
};

DLX<729,240000> g;

void pre_calc() {
    g.init(9*9*9, 9*9*4, 9*9);
    int cnt = 0, b, t1, t2, t3, t4;
    for(int i = 0; i < 9; ++i) {
        for(int j = 0; j < 9; ++j) {
            b = (i / 3) * 3 + (j / 3);
            for(int k = 1; k <= 9; ++k) {
                t1 = 81*0 + i * 9 + k;
                t2 = 81*1 + j * 9 + k;
                t3 = 81*2 + b * 9 + k;
                t4 = 81*3 + i * 9 + j + 1;
                cnt += 1;
                g.add(cnt, t1);
                g.add(cnt, t2);
                g.add(cnt, t3);
                g.add(cnt, t4);
            }
        }
    }
}

int A[9][9];

int main() {
    freopen("in","r",stdin);
    int T; scanf("%d", &T);
    char s[12];
    while(T--) {
        pre_calc();
        for(int i = 0; i < 9; ++i) {
            scanf("%s", s);
            for(int j = 0; j < 9; ++j) {
                A[i][j] = s[j] - '0';
                if(A[i][j] != 0) {
                    g.select(i * 81 + j * 9 + A[i][j]);
                }
            }
        }
        if(!g.main()) {
            puts("Could not complete this grid.");
            if(T)puts("");
            continue;
        }
        for(int i = 0; i < g.ansd; ++i) {
            int t = g.ans[i] - 1;
            int r = t / 81; t %= 81;
            int c = t / 9; t %= 9;
            int v = t + 1;
            A[r][c] = v;
        }
        for(int i = 0; i < 9; ++i) {
            for(int j = 0; j < 9; ++j) 
                printf("%d", A[i][j]);
            printf("\n");
        }
        if(T)puts("");
    }
}

