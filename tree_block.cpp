/* 块状链表模板
 * blocked link
 * code for HDU 3601
 */

# include <iostream> 
# include <cstdio>
# include <cstring>
# include <cmath>

using namespace std;
#define MAXN 32000
#define MAXE 128000
#define INF 0x3f3f3f3f
struct node {
	int x;
	node *next;
} *h[MAXN], *g[MAXN], es[MAXE];

int cnt, Limit;
void add_edge(int x,int y) {
	node *p = &es[cnt++];
	p->x = y;
	p->next = h[x];
	h[x] = p;
}

void add_edge2(int x,int y) {
	node *p = &es[cnt++];
	p->x = y;
	p->next = g[x];
	g[x] = p;
}

int fa[MAXN], dep[MAXN], own[MAXN];
long long sum[MAXN];
int mun[MAXN], size[MAXN];
int w[MAXN];
void build(int x,int f,int d) {
	fa[x] = f; dep[x] = d; 
	sum[x] = 0; mun[x] = -INF;
	int t = own[x];
	for(node *p=h[x]; p; p=p->next) {
		if(p->x != f) {
			if(size[t] < Limit) {
				++size[t];
				own[p->x] = t;
				add_edge2(x, p->x);
			}
			build(p->x, x, d+1);
		}
	}
}

void dfs(int x,long long s,int m) {
	s += w[x]; sum[x] = s;
	m =max(m, w[x]); mun[x] = m;
	for(node *p=g[x];p;p=p->next)
		dfs(p->x, s, m);
}

void change(int x,int y) {
	w[x] = y;
	if(x == own[x]) dfs(x, 0, -INF);
	else dfs(x, sum[fa[x]], mun[fa[x]]);
}

void get_answer(int a,int b,long long &s,int &m) {
	s = 0; m = -INF;
	while(a != b) {
		if(dep[a] < dep[b]) swap(a, b);
		if(own[a] == own[b]) {
			s += w[a]; m = max(m, w[a]);
			a = fa[a];
		} else {
			if(dep[own[a]] < dep[own[b]]) swap(a, b);
			s += sum[a]; m =max(m, mun[a]);
			a = fa[own[a]];
		}
	}
	s += w[a]; m =max(m, w[a]);
}

char cmd[120];
int main() {
	int n, x, y, Q, m; 
	long long s;
	while(~scanf("%d", &n)) {
		cnt = 0;
		memset(h, 0, sizeof(h));
		memset(g, 0, sizeof(g));
		memset(size, 0, sizeof(size));
		Limit = (int)(sqrt(n)+1);
		for(int i=1;i<n;++i) {
			scanf("%d%d", &x, &y);
			add_edge(x, y);
			add_edge(y, x);
		}
		for(int i=1; i<=n; ++i) {
			scanf("%d", &w[i]);
			own[i] = i;
		}
		build(1, -1, 1);
		for(int i=1; i<=n; ++i) 
			if(own[i]==i) dfs(i, 0, -INF);
		scanf("%d", &Q);
		while(Q--) {
			scanf("%s%d%d",cmd,&x,&y);
			if(cmd[0]=='Q') {
				get_answer(x,y,s,m);
				if(cmd[1]=='M') 
					printf("%d\n", m);
				else 
					printf("%I64d\n", s);
			} else change(x,y);
		}
	}
}
