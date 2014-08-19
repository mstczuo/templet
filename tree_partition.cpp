/*
 * HDU 3588 -- 树分治
 * pragma is only avilable using C++ instead of G++
 */
#pragma comment(linker, "/STACK:25165824")
# include <iostream>
# include <cstdio>
# include <algorithm>
# include <cstring>
using namespace std;

const int MAXN=100010;
const int MAXE=MAXN*2;
typedef long long LL;
LL ans = 0;

struct node{
    int x, w;
    node *next;
} es[MAXE], *h[MAXN];
int cnt, tot, n, m, L;
int s[MAXN], w[MAXN];
bool v[MAXN];

void add_edge(int x,int y,int w) {
    node *p = &es[cnt++];
    p->x = y; p->w = w;
    p->next = h[x]; h[x]=p;
}

int size(int e,int fa) {
    int &c = s[e]; c = 1;
    for(node*p=h[e];p;p=p->next)
        if(p->x!=fa&&!v[p->x]) 
            c+=size(p->x,e);
    return c;
}

int center(int e,int c,int fa) {
    for(node*p=h[e];p;p=p->next)
        if(p->x!=fa&&!v[p->x]&&s[p->x]>c/2)
            return center(p->x,c,e);
    return e;
}

void dfs(int e,int fa,int d) {
    w[tot++] = d;
    for(node*p=h[e];p;p=p->next)
        if(p->x!=fa&&!v[p->x]) dfs(p->x,e,d+p->w);
}

LL count(int x,int dist) {
    LL ret = 0;
    tot = 0; dfs(x, -1, dist);
    sort(w, w+tot);
    for(int i=0, j=tot-1; i<j; i++) {
        while(i<j && w[i]+w[j]>L)j--;
        if(i<j)ret+=j-i;
    }
    return ret;
}

void solve(int e) {
    e = center(e, size(e,-1), -1);
    v[e] = true;
    ans += count(e, 0);
    for(node*p=h[e];p;p=p->next)
        if(!v[p->x]) ans-=count(p->x,p->w);
    for(node*p=h[e];p;p=p->next)
        if(!v[p->x]) solve(p->x);
}

int main() {
    while(~scanf("%d%d",&n,&m)) {
        cnt = 0; ans = 0;
        memset(h, 0, sizeof(h));
        memset(v, 0, sizeof(v));
        while(m--) {
            int u, v, w;
            scanf("%d%d%d",&u,&v,&w);
            add_edge(u, v, w);
            add_edge(v, u, w);
        }
        scanf("%d", &L);
        solve(1);
        printf("%I64d\n", ans+n);
    }
}

