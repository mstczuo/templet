/*
 * Heavy Light Decomposition
 */
# include <iostream>
# include <cstdio>
# include <cstring>

using namespace std;

#define MAXN 100010
#define MAXE 200020
#define MAXT 400040

struct Mark {
    Mark() {
        flag = false;
        col = 0;
    }
    Mark(int c) {
        flag = true;
        col = c;
    }
    bool flag;
    int col;
};

struct Info {
    int lc, rc, cnt;
    Info():cnt(-1) {}
    Info(int a,int b,int c):lc(a),rc(b),cnt(c){}
    void set(int col) {
        lc = rc = col;
        cnt = 1;
    }
};

Info operator+(const Info&l,const Info&r) {
    if(l.cnt == -1) return r;
    if(r.cnt == -1) return l;
    return Info(l.lc, r.rc, l.cnt+r.cnt-(l.rc==r.lc));
}

struct Node {
    Info info;
    Mark mark;
    Node *l, *r;
    int L, R;
    void push() {
        if(mark.flag) {
            if(l)l->apply(mark.col);
            if(r)r->apply(mark.col);
            mark.flag = false;
        }
    }
    void apply(int m) {
        this->mark = Mark(m);
        info = Info(m,m,1);
    }
    void upd() {
        info = l->info + r->info;
    }
}ns[MAXT], *ne, *root;

struct node {
    int x;
    node *next;
}*h[MAXN], es[MAXE], *ee;

int w[MAXN], top[MAXN], Fa[MAXN];
int dep[MAXN], size[MAXN], son[MAXN];
int ic[MAXN], cc[MAXN];
int tid;

void add_edge(int x,int y) {
    ee->x = y; ee->next = h[x]; h[x] = ee++;
    ee->x = x; ee->next = h[y]; h[y] = ee++;
}

Node* build(int L,int R) {
    Node *u = ne++;
    u->L = L;
    u->R = R;
    u->mark = Mark();
    if(L == R) {
        int c = cc[L];
        u->info = (Info){c,c,1};
        u->l = u->r = NULL;
        return u;
    }
    u->l = build(L, (L+R)/2);
    u->r = build((L+R)/2+1, R);
    u->upd();
    return u;
}

void init() {
    ne = ns; 
    root = NULL;
    ee = es;
    memset(h, 0, sizeof(h));
    tid = 0;
}

void DFS(int u,int fa,int D) {
    Fa[u] = fa; dep[u] = D; 
    size[u] = 1; son[u] = -1;
    int Max = 0;
    for(node *p = h[u]; p; p=p->next) {
        if(p->x == fa) continue;
        DFS(p->x, u, D+1);
        size[u] += size[p->x];
        if(size[p->x] > Max) {
            Max = size[p->x];
            son[u] = p->x;
        }
    }
}

void make(int u,bool isTop) {
    w[u] = ++tid; cc[tid] = ic[u];
    top[u] = isTop ? u:top[Fa[u]];
    if(son[u] == -1) return;
    make(son[u], false);
    for(node *p = h[u]; p; p=p->next) 
        if(p->x != Fa[u] && p->x != son[u])
            make(p->x, true);
}

Info get(int L,int R,Node *u = root) {
    u->push();
    if(u->R < L || R < u->L) return Info();
    if(L <= u->L && u->R <= R) return u->info;
    return get(L, R, u->l) + get(L, R, u->r);
}

void set(int L,int R,int c,Node *u = root) {
    u->push();
    if(u->R < L || R < u->L) return;
    if(L <= u->L && u->R <= R) {
        u->apply(c);
        return;
    }
    set(L, R, c, u->l);
    set(L, R, c, u->r);
    u->upd();
}

Info query(int x,int y) {
    int fx, fy;
    Info L, R;
    while(top[x] != top[y]) {
        fx = top[x], fy = top[y];
        if(dep[fx] > dep[fy]) {
            L = get(w[fx], w[x]) + L;
            x = Fa[fx];
        } else {
            R = get(w[fy], w[y]) + R;
            y = Fa[fy];
        }
    }
    if(w[x] <= w[y]) {
        R = get(w[x], w[y]) + R;
    } else {
        L = get(w[y], w[x]) + L;
    }
    swap(L.lc, L.rc);
    return L + R;
}

void change(int x,int y,int c) {
    int fx, fy;
    while(top[x] != top[y]) {
        fx = top[x], fy = top[y];
        if(dep[fx] > dep[fy]) {
            set(w[fx], w[x], c);
            x = Fa[fx];
        } else {
            set(w[fy], w[y], c);
            y = Fa[fy];
        }
    }
    if(w[x] > w[y]) swap(x, y);
    set(w[x], w[y], c);
}

int main() {
    freopen("e.in","r",stdin);
    init();
    int n, m, x, y, c;
    char cmd[12];
    scanf("%d%d", &n, &m);
    for(int i = 1; i <= n; ++i) 
        scanf("%d", &ic[i]);
    for(int i = 1; i < n; ++i) {
        scanf("%d%d", &x, &y);
        add_edge(x, y);
    }
    DFS(1, 0, 1);
    make(1, true);
    root = build(1, n);
    while(m--) {
        scanf("%s", cmd);
        if(cmd[0] == 'C') {
            scanf("%d%d%d", &x, &y, &c);
            change(x, y, c);
        } else {
            scanf("%d%d", &x, &y);
            printf("%d\n", query(x, y).cnt);
        }
    }
}
