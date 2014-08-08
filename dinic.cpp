# include <iostream>
# include <cstdio>
# include <cstring>
# include <queue>
using namespace std;

#define MAXN 10000
#define MAXE 100000
#define inf 0x3fffffff
struct Dinic{
    struct node {
        int x;
        int f;
        int next;
    };

    int h[MAXN], d[MAXN];
    node e[MAXE];
    int S, T, cnt;

    void add_edge(int x,int y,int f) {
        e[cnt].x = y; e[cnt].f = f;
        e[cnt].next = h[x]; h[x] = cnt++;
        e[cnt].x = x; e[cnt].f = 0;
        e[cnt].next = h[y]; h[y] = cnt++;
    }

    void init() {
        memset(h, 0, sizeof(h));
        cnt = 2;
    }

    bool build() {
        memset(d, -1, sizeof(d));
        queue<int> q; q.push(S); 
        d[S] = 0;
        while(!q.empty()) {
            int x = q.front(); q.pop();
            for(int i=h[x]; i; i=e[i].next) {
                if(e[i].f && d[e[i].x] == -1) {
                    d[e[i].x] = d[x] + 1;
                    q.push(e[i].x);
                }
            }
        }
        return d[T]!=-1;
    }

    int find(int x,int low = inf) {
        if(x == T) return low;
        int w = 0, ret = 0;
        for(int i=h[x]; i&&w<low; i=e[i].next)
            if(e[i].f && d[e[i].x]==d[x]+1) {
                ret = find(e[i].x, min(e[i].f, low-w));
                e[i].f -= ret; e[i^1].f += ret; w += ret;
            }
        if(!w) d[x] = -1;
        return w;
    }

    int main(int s,int t) {
        S = s; T = t;
        int ans = 0;
        while(build()) 
            ans += find(S, inf);
        return ans;
    }
}D;

int main() {
    int u, v, w, m, n;
    while(~scanf("%d%d",&m,&n)) {
        D.init();
        for(int i=0; i<m; ++i) {
            scanf("%d%d%d",&u,&v,&w);
            D.add_edge(u,v,w);
        }
        printf("%d\n",D.main(1,n));
    }
}
