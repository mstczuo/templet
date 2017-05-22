/* BZOJ 1095*/
/* Dynamic tree_partition*/

# include <bits/stdc++.h>

#define rep(i, n) for(int i = 0; i < (n); ++i)
#define dep(i, n) for(int i = (n) - 1; i >= 0; --i)
#define ri(x) scanf("%d", &x)

using namespace std;
const int maxn = 100010;

struct sq {
	priority_queue<int> q, del;
	void push(int x) { q.push(x); }
	void erase(int x) { del.push(x); }
	int top() {
		if(sz() == 0) return -1;
		while(!del.empty() && del.top() == q.top()) { del.pop(); q.pop(); }
		return q.top();
	}
	void pop() {
		while(!del.empty() && del.top() == q.top()) { del.pop(); q.pop(); }
		q.pop();
	}
	int top2() {
		if(sz() >= 2) {
			int t = top(); pop();
			int ret = top(); push(t);
			return t + ret;
		}
		return -1;
	}
	int sz() { return q.size() - del.size(); }
} B[maxn], C[maxn], ans;

vector<int> dist[maxn], g[maxn];
bool vis[maxn];
int sz[maxn];
int fa[maxn];

void upd_dist(int u,int par,int dis = 0) {
	dist[u].push_back(dis);
	for(auto v: g[u]) if(!vis[v] && v!=par) {
		upd_dist(v, u, dis + 1);
	}
}

void dfs(int u,int par) {
	sz[u] = 1;
	for(auto v: g[u]) if(!vis[v] && v!=par) {
		dfs(v, u); sz[u] += sz[v];
	}
}

int _lim;
int dfs2(int u,int par) {
	for(auto v: g[u]) if(!vis[v] && v!=par) 
		if(sz[v] > _lim) return dfs2(v, u);
	return u;
}

int center(int u) {
	dfs(u, u);
	_lim = sz[u] / 2;
	return dfs2(u, u);
}

void divide(int u,int par) {
	fa[u] = par; vis[u] = true;
	upd_dist(u, u);
	for(auto v: g[u]) if(!vis[v]) {
		int c = center(v);
		divide(c, u);
	}
}

int la[maxn]; // last_ans
void Qupd(int i) {
	int last = la[i];
	int cur = B[i].top2();
	if(cur != last) {
		if(~last) ans.erase(last);
		if(~cur) ans.push(cur);
	}
	la[i] = cur;
}

void turn_off(int u) {
	auto it = dist[u].end() - 1;
	B[u].push(0); Qupd(u);
	for(int r = u, v = fa[u]; ~v; r = v, v = fa[v]) {
		int tmp = C[r].top();
		C[r].push(*--it);
		if(*it > tmp) {
			if(~tmp) B[v].erase(tmp);
			B[v].push(*it);
			Qupd(v);
		}
	}
}

void turn_on(int u) {
	auto it = dist[u].end() - 1;
	B[u].erase(0); Qupd(u);
	for(int r = u, v = fa[u]; ~v; r = v, v = fa[v]) {
		C[r].erase(*--it);
		int tmp = C[r].top();
		if(*it > tmp) {
			if(~tmp) B[v].push(tmp);
			B[v].erase(*it);
			Qupd(v);
		}
	}
}

int n, m, cnt;
char buf[120];
int state[maxn];
int main() {
	ri(n);
	rep(i, n) dist[i].reserve(18);
	memset(la, -1, sizeof(la));
	rep(i, n-1) {
		int u, v; ri(u), ri(v);
		--u, --v;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	divide(center(0), -1);
	cnt = n;
	rep(i, n) turn_off(i), state[i] = 1;
	ri(m);
	while(m--) {
		scanf("%s", buf);
		if(buf[0] == 'G') {
			if(cnt <= 1) printf("%d\n", cnt - 1);
			else printf("%d\n", ans.top());
		} else {
			int i; ri(i); --i;
			state[i] ? turn_on(i) : turn_off(i);
			state[i] ? --cnt : ++cnt;
			state[i] ^= 1;
		}
	}
	return 0;
}

