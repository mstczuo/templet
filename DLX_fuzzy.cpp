#include <iostream> 
#include <algorithm>
#include <cstring>
#include <vector>
#include <cstdio>
#include <queue>
#include <set>
#include <map>
using namespace std;
typedef long long ll;
const int MAX_NODE = 8000;
const int MAXM = 70;
const int MAXN = 70;
int K;

template<int N,int M>
struct DLX {
    int n, m, size;
    int U[M], D[M], L[M], R[M], Row[M], Col[M];
    int H[M], S[M];
    int ands, ans[N];
    void init(int _n,int _m) {
        n = _n; m = _m;
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
        for(int i = D[c]; i!=c; i=D[i])
            L[R[i]]=L[i],R[L[i]]=R[i];
    }
    void resume(int c) {
        for(int i = U[c]; i!=c; i=U[i])
            L[R[i]]=R[L[i]]=i;
    }
    bool v[M];
    int f() {
        int ret = 0;
        for(int c=R[0]; c; c=R[c]) v[c]=true;
        for(int c=R[0]; c!=0; c=R[c])
            if(v[c]) {
                ret++;
                v[c]=false;
                for(int i=D[c];i!=c;i=D[i])
                    for(int j=R[i];j!=i;j=R[j])
                        v[Col[j]]=false;
            }
        return ret;
    }
    bool Dance(int d) {
        if(d + f() > K) return false;
        if(R[0] == 0) return true;
        int c = R[0];
        for(int i = R[0]; i != 0;i = R[i])
            if(S[i] < S[c]) c = i;
        for(int i = D[c]; i != c;i = D[i]) {
            remove(i);
            for(int j=R[i];j!=i;j=R[j]) remove(j);
            if(Dance(d+1)) return true;
            for(int j=L[i];j!=i;j=L[j]) resume(j);
            resume(i);
        }
        return false;
    }
};

DLX<MAXN,MAX_NODE> g;

struct Point {
    int x, y;
    void read() {
        scanf("%d%d", &x, &y);
    }
} city[MAXN];

ll dis(const Point&a,const Point&b) {
    return (ll)abs(a.x-b.x) + (ll)abs(a.y-b.y);
}

int main() {
    freopen("e.in","r",stdin);
    int T; scanf("%d", &T);
    for(int cas = 1; cas <= T; ++cas) {
        int n;
        scanf("%d%d", &n, &K);
        for(int i = 1; i <= n; ++i) city[i].read();
        ll L = -1, R = 6000000000LL, Mid;
        while(R - L > 1) {
            Mid = (L + R) >> 1;
            g.init(n, n);

            for(int i = 1; i <= n; ++i)
                for(int j =1; j <= n; ++j)
                    if(dis(city[i], city[j]) <= Mid)
                        g.add(i, j);
            if(g.Dance(0)) R = Mid;
            else L = Mid;
        }
        printf("Case #%d: %lld\n", cas, R);
    }
    return 0;
}

