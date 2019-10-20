#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
const int maxn = int(1e5 + 10);

struct TwoSAT {
	int n, s[maxn * 2], c;
	bool mark[maxn * 2];
	vector<int> g[maxn * 2];

	void init(int n) {
		this->n = n;
		memset(mark, false, sizeof(mark));
		for (int i = 0; i < 2 * n; i++) g[i].clear();
	}

	void addClause(int x, int xflag, int y, int yflag) {
		x = x * 2 + xflag;
		y = y * 2 + yflag;
		g[x^1].push_back(y);
		g[y^1].push_back(x);
	}

	bool dfs (int u) {
		if (mark[u^1]) return false;
		if (mark[u]) return true;
		mark[u] = true;
		s[c++] = u;
		for (int i = 0; i < g[u].size(); i++)
			if (!dfs(g[u][i])) return false;
		return true;
	}

	bool solve () {
		for (int i = 0; i < 2 * n; i += 2) {
			if (!mark[i] && !mark[i+1]) {
				c = 0;
				if (!dfs(i)) {
					while (c) mark[s[--c]] = false;
					if (!dfs(i+1)) return false;
				}
			}
		}
		return true;
	}
}solver;

struct Point {
	ll x, y;
	void read() { scanf("%lld%lld", &x, &y); }
	bool operator < (const Point& a) const { return x < a.x || (x == a.x && y < a.y); }
}P[4], A[maxn], B[maxn], C[maxn], D[maxn];

int N;

int mul(Point a, Point b, Point c) {
	ll ret = (b.x-a.x) * (c.y-a.y) - (b.y-a.y) * (c.x-a.x);
	if (ret == 0) return 0;
	return ret > 0 ? 1 : -1;
}

bool judge (Point a, Point b, Point c, Point d) {
	if (min(a.x, b.x) <= max(c.x, d.x) &&
		min(a.y, b.y) <= max(c.y, d.y) &&
		min(c.x, d.x) <= max(a.x, b.x) &&
		min(c.y, d.y) <= max(a.y, b.y) &&
		mul(a, c, b) * mul(a, d, b) <= 0 &&
		mul(c, a, d) * mul(c, b, d) <= 0) 
		return true;
	return false;
}

void init() {
	solver.init(2 * N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 4; j++) P[j].read();
		sort(P, P + 4);
		D[i] = P[0];
		A[i] = P[1];
		C[i] = P[2];
		B[i] = P[3];
        
        solver.addClause(i * 2, 0, i * 2 + 1, 0);

		for (int j = 0; j < i; j++) {
			if (judge(A[i], C[i], A[j], C[j]))
				solver.addClause(i * 2 + 1, 1, j * 2 + 1, 1);
			if (judge(A[i], C[i], B[j], D[j]))
				solver.addClause(i * 2 + 1, 1, j * 2, 1);
			if (judge(B[i], D[i], A[j], C[j]))
				solver.addClause(i * 2, 1, j * 2 + 1, 1);
			if (judge(B[i], D[i], B[j], D[j]))
				solver.addClause(i * 2, 1, j * 2, 1);
		}
	}
}

int main () {
	while (scanf("%d", &N) == 1 && N) {
		init();
		printf("%s\n", solver.solve() ? "YES" : "NO");
	}

	return 0;
}