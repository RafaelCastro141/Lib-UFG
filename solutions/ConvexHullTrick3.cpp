#include <bits/stdc++.h>

using namespace std;

typedef long long	ll;

const int N = int(1e5 + 10);
int subtree[N];
ll a[N], b[N];
ll dp[N];
bool is_leaf[N];
vector< int > adj[N];

bool Q;
struct Line {
    mutable ll k, m, p;
    bool operator<(const Line& o) const {
        return Q ? p < o.p : k < o.k;
    }
};
struct LineContainer : multiset<Line> {
    const ll inf = LLONG_MAX;
    ll div(ll a, ll b){
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) { x->p = inf; return false; }
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    ll query(ll x) {
        assert(!empty());
        Q = 1; auto l = *lower_bound({0,0,x}); Q = 0;
        return l.k * x + l.m;
    }
};

int dfs(int u, int ft){
	is_leaf[u] = 1;

	for(auto v: adj[u]){
		if(v == ft){
			continue;
		}

		is_leaf[u] = 0;
	}

	if(is_leaf[u]){
		return subtree[u] = 1;
	}

	subtree[u] = 1;

	for(auto v: adj[u]){
		if(v == ft){
			continue;
		}

		subtree[u] += dfs(v, u);
	}

	return subtree[u];
}

void dfs1(int u, int ft, LineContainer &cur){
	if(is_leaf[u]){
		dp[u] = 0LL;
		cur.add(-b[u], -dp[u]);
		
		return;
	}

	int mx = 0;
	int big_son = 0;

	for(auto v: adj[u]){
		if(v == ft){
			continue;
		}

		if(subtree[v] > mx){
			big_son = v;
			mx = subtree[v];
		}
	}

	dfs1(big_son, u, cur);

	for(auto v: adj[u]){
		if(v == big_son || v == ft){
			continue;
		}

		LineContainer tmp;
		dfs1(v, u, tmp);

		for(auto d: tmp){
			//printf("aqui!\n");
			cur.add(d.k, d.m);
		}
	}

	dp[u] = -cur.query(a[u]);
	cur.add(-b[u], -dp[u]);
}

int main(){
	int n;

	scanf("%d", &n);

	for(int i = 1 ; i <= n ; i++){
		scanf("%lld", &a[i]);
	}

	for(int i = 1 ; i <= n ; i++){
		scanf("%lld", &b[i]);
	}

	for(int i = 1 ; i < n ; i++){
		int ui, vi;

		scanf("%d %d", &ui, &vi);

		adj[ui].push_back(vi);
		adj[vi].push_back(ui);
	}

	LineContainer cur;
	dfs(1, 1);
	dfs1(1, 1, cur);

	for(int i = 1 ; i <= n ; i++){
		printf("%lld ", dp[i]);
	}

	printf("\n");

	return 0;
}