#include <bits/stdc++.h>

using namespace std;

class Edge{
public:
	int a, b;
};

const int N = int(2e5 + 10);
typedef long long	ll;

vector< Edge > edges;
vector< vector< int > > adj;
vector< int > parent, depth, heavy, head, pos;
int cur_pos;
int n;
int carry[N];
int idx[N];
ll arr[N];

int dfs(int v){
	int size = 1;
	int max_c_size = 0;

	for(int c: adj[v]){
		if(c != parent[v]){
			parent[c] = v, depth[c] = depth[v] + 1;
			int c_size = dfs(c);
			size += c_size;

			if(c_size > max_c_size){
				max_c_size = c_size, heavy[v] = c;
			}
		}
	}

	return size;
}

void decompose(int v, int h){
	idx[cur_pos] = v;
	head[v] = h, pos[v] = cur_pos++;

	if(heavy[v] != -1){
		decompose(heavy[v], h);
	}

	for(int c: adj[v]){
		if(c != parent[v] && c != heavy[v]){
			decompose(c, c);
		}
	}
}

class Node{
public:
	ll pref, suf, best, total;
};

const ll inf = ll(1e10);
ll lazzy[N << 2];
int lazzy1[N << 2];
Node segtree[N << 2];

void shift(int node, ll el){
	int lf = node << 1;
	int rg = lf + 1;

	ll val = max(0LL, lazzy[node] * el);
	segtree[node].pref = val;
	segtree[node].suf = val;
	segtree[node].best = val;
	segtree[node].total = lazzy[node] * el;

	lazzy[lf] = lazzy[node];
	lazzy[rg] = lazzy[node];
	lazzy1[lf] = lazzy1[rg] = 1;

	lazzy1[node] = 0;
}

Node merge(Node lf, Node rg){
	Node r;

	if(lf.total == -inf){
		return rg;
	}

	if(rg.total == -inf){
		return lf;
	}

	r.pref = max(lf.pref, lf.total + rg.pref);
	r.suf = max(rg.suf, rg.total + lf.suf);
	r.best = max({r.pref, r.suf, lf.best, rg.best, lf.suf + rg.pref});
	r.total = lf.total + rg.total;

	return r;
}

void build(int node, int i, int j){
	if(i == j){
		segtree[node].total = arr[idx[i] + 1];
		segtree[node].suf = max(0LL, arr[idx[i] + 1]);
		segtree[node].pref = max(0LL, arr[idx[i] + 1]);
		segtree[node].best = max(0LL, arr[idx[i] + 1]);
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;

		build(lf, i, mid);
		build(rg, mid + 1, j);
		
		segtree[node] = merge(segtree[lf], segtree[rg]);
	}
}

void update(int node, int i, int j, int l, int r, ll val){
	if(lazzy1[node]){
		shift(node, j - i + 1);
	}

	if(i > r || j < l) return;

	if(l <= i && j <= r){
		lazzy[node] = val;
		lazzy1[node] = 1;
		shift(node, j - i + 1);
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;

		update(lf, i, mid, l, r, val);
		update(rg, mid + 1, j, l, r, val);

		segtree[node] = merge(segtree[lf], segtree[rg]);
	}
}

Node segment_tree_query(int node, int i, int j, int l, int r){
	if(lazzy1[node]){
		shift(node, j - i + 1);
	}

	if(l > r) return {-inf, -inf, -inf, -inf};

	if(i > r || j < l){
		return {-inf, -inf, -inf, -inf};
	}

	if(l <= i && j <= r){
		return segtree[node];
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;

		return merge(segment_tree_query(lf, i, mid, l, r), 
			segment_tree_query(rg, mid + 1, j, l, r));
	}
}

void init(){
	int n = adj.size();
	parent = vector< int >(n);
	depth = vector< int >(n);
	heavy = vector< int >(n, -1);
	head = vector< int >(n);
	pos = vector< int >(n);

	cur_pos = 1;

	dfs(0);

	decompose(0, 0);
	build(1, 1, n);
}

Node merge2(Node lf, Node rg){
	Node r;

	r.best = max({lf.best, rg.best, rg.pref + lf.pref});

	return r;
}

ll query(int a, int b){
	Node res[2] = {{0LL, 0LL, 0LL, 0LL}, {0LL, 0LL, 0LL, 0LL}};
	int c = 0;

	for(; head[a] != head[b] ; b = parent[head[b]]){
		if(depth[head[a]] > depth[head[b]]){
			swap(a, b);
			c = !c;
		}

		res[c] = merge(segment_tree_query(1, 1, n, pos[head[b]], pos[b]), res[c]);
	}

	if(pos[a] > pos[b]){
		c = !c;
		swap(a, b);
	}
	
	res[c] = merge(segment_tree_query(1, 1, n, pos[a], pos[b]), res[c]);
	res[c] = merge2(res[c], res[!c]);

	return res[c].best;
}

void updatet(int a, int b, ll val){
	for(; head[a] != head[b] ; b = parent[head[b]]){
		if(depth[head[a]] > depth[head[b]]){
			swap(a, b);
		}

		update(1, 1, n, pos[head[b]], pos[b], val);
	}

	if(pos[a] > pos[b]){
		swap(a, b);
	}

	update(1, 1, n, pos[a], pos[b], val);
}

int main(){
	scanf("%d", &n);

	for(int i = 1 ; i <= n ; i++){
		scanf("%lld", &arr[i]);
	}

	adj.resize(n);

	for(int i = 1 ; i < n ; i++){
		int ui, vi;
		scanf("%d %d", &ui, &vi);

		ui--, vi--;
		adj[ui].push_back(vi);
		adj[vi].push_back(ui);
	}

	int q;

	init();

	scanf("%d", &q);

	while(q--){
		int t;

		scanf("%d", &t);

		if(t == 1){
			int a, b;

			scanf("%d %d", &a, &b);
			
			a--, b--;

			printf("%lld\n", query(a, b));
		}else{
			int a, b;
			ll c;

			scanf("%d %d %lld", &a, &b, &c);

			a--, b--;

			updatet(a, b, c);
		}
	}

	return 0;
}
