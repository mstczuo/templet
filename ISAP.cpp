# include <iostream>
# include <cstdio>
# include <cstring>
# include <queue>
using namespace std;
#define MAXN 1600
#define MAXE 4800000
#define inf 0x3fffffff

struct ISAP {
    int n, cnt, s, t;
    struct node{
        int x, f, next;
    } e[MAXE];

    int g[MAXN], h[MAXN], vh[MAXN];

    int aug(int u, int m) {
        if (u == t) return m;
        int d = m;
        for(int i=g[u]; i; i=e[i].next) {
            int v = e[i].x;
            if (e[i].f && h[u] == h[v] + 1) {
                int f = aug(v, min(e[i].f, d));
                e[i].f -= f; e[i^1].f += f; d -= f;
                if (h[s] == n || !d) return m - d;
            }
        }
        int w = n;
        for(int i=g[u]; i; i=e[i].next) 
            if(e[i].f) w=min(w, h[e[i].x]+1);
        if (h[u] < w) --vh[h[u]]? ++vh[h[u]=w]: h[s] = n;
        return m - d;
    }

    void init(int n)
    {
        this->n = n; cnt = 2;
        memset(g, 0, sizeof(g));
    }

    void add_edge(int x,int y,int f) {
        e[cnt].x = y; e[cnt].f = f;
        e[cnt].next = g[x]; g[x] = cnt++;
        e[cnt].x = x; e[cnt].f = 0;
        e[cnt].next = g[y]; g[y] = cnt++;
    }

    int max_flow(int s, int t)
    {
        if (s == t) return 0;
        this->s = s; this->t = t;
        memset(h, 0, sizeof(h));
        memset(vh, 0, sizeof(vh)); vh[0] = n;
        int flow = 0;
        while (h[s] != n) flow += aug(s, inf);
        return flow;
    }
} isap;

int main() {
    int T, cas=0; scanf("%d", &T);
    while(T--) {
        int n, m, x, y, w;
        scanf("%d%d",&n,&m);
        isap.init(n);
        while(m--) {
            scanf("%d%d%d",&x,&y,&w);
            isap.add_edge(x,y,w);
        }
        printf("Case %d: %d\n",++cas,isap.max_flow(1,n));
    }
}
