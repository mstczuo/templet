/* HDU 3957 */
#include <cstring>
#include <cstdio>
#include <vector>
using namespace std;
typedef long long ll;

const int MAXN = 100;

template<int N,int M>
struct DLX {
    int n, m, size;
    int U[M], D[M], L[M], R[M], Row[M], Col[M];
    int H[M], S[M];
    int ansd, ans[N];
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
        ansd = ~0U>>1;
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

    void deleteCol(int c) {
        L[R[c]]=L[c],R[L[c]]=R[c];
        for(int i = D[c]; i!=c; i=D[i]) {
            for(int j = R[i]; j!=i; j=R[j])
                U[D[j]]=U[j],D[U[j]]=D[j],--S[Col[j]];
        }
    }

    void resumeCol(int c) {
        for(int i = U[c]; i!=c; i=U[i]) {
            for(int j = R[i]; j!=i; j=R[j])
                U[D[j]]=D[U[j]]=j,++S[Col[j]];
        }
        L[R[c]]=R[L[c]]=c;
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
        for(int c=R[0]; c && c <= n; c=R[c]) v[c]=true;
        for(int c=R[0]; c && c <= n; c=R[c]) if(v[c]) {
            ret++;
            v[c]=false;
            for(int i=D[c];i!=c;i=D[i])
                for(int j=R[i];j!=i;j=R[j])
                    v[Col[j]]=false;
        }
        return ret;
    }

    void Dance(int d) {
        if(d + f() >= ansd) return;
        if(!R[0] || R[0] > n) {
            if(ansd > d) ansd = d;
            return;
        }
        int Min = ~0U>>1, c = R[0];
        for(int i = R[0]; i != 0 && i <= n; i = R[i])
            if(S[i] < Min) Min = S[i], c = i;
        for(int i = D[c]; i != c; i = D[i]) {
            remove(i);
            for(int j = R[i]; j != i; j = R[j])
                if(Col[j] <= n) remove(j);
            for(int j = R[i]; j != i; j = R[j])
                if(Col[j] > n) deleteCol(Col[j]);
            Dance(d + 1);
            for(int j = L[i]; j != i; j = L[j])
                if(Col[j] > n) resumeCol(Col[j]);
            for(int j = L[i]; j != i; j = L[j])
                if(Col[j] <= n) resume(j);
            resume(i);
        }
    }
    
};

DLX<200,2000> g;

int ps[MAXN], id[MAXN][3];
vector<pair<int,int> > kill[MAXN];
int main() {
    freopen("in","r",stdin);
    int T; scanf("%d", &T);
    for(int cas = 1; cas <= T; ++cas) {
        int r, n = 0, m = 0;
        scanf("%d", &r);
        for(int i = 0; i < r; ++i) {
            scanf("%d", &ps[i]);
            for(int j = 0; j < ps[i]; ++j) {
                id[i][j] = ++n;
                kill[n].clear();
                scanf("%d", &m);
                for(int k = 0; k < m; ++k) {
                    pair<int, int> t;
                    scanf("%d%d", &t.first, &t.second);
                    kill[n].push_back(t);
                }
            }
        }
        g.init(n, n + r);
        for(int i = 0; i < r; ++i) 
            for(int j = 0; j < ps[i]; ++j) {
                g.add(id[i][j], n + i + 1);
                for(int k = 0; k < ps[i]; ++k) 
                    g.add(id[i][j], id[i][k]);
            }
        for(int i = 1; i <= n; ++i) 
            for(size_t k = 0; k < kill[i].size(); ++k) {
                int j = id[kill[i][k].first][kill[i][k].second];
                g.add(i, j);
            }
        g.Dance(0);
        printf("Case %d: %d\n", cas, g.ansd);
    }
}
