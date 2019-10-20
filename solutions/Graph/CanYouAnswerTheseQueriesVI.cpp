#include <bits/stdc++.h>
 
using namespace std;
 
typedef pair< int, int >	pii;
const int N = int(1e5 + 10);
vector< int > adj[N];
vector< int > parent, depth, heavy, head, pos;
int sz[N], idx[N];
pii lazzy[N << 3];
int cur_pos;
pii segtree1[N << 3];
pii segtree2[N << 3];
int n;
int pivot = N - 5;
int cor[N];
 
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
 
	sz[v] = size;
 
	return size;
}
 
void decompose(int v, int h){
	idx[cur_pos] = v;
	head[v] = h, pos[v] = cur_pos++;
 
	// printf("head[%d] = %d, %d\n", v, h, heavy[v]);
 
	if(heavy[v] != -1){
		decompose(heavy[v], h);
	}
 
	for(int c: adj[v]){
		if(c != parent[v] && c != heavy[v]){
			decompose(c, c);
		}
	}
}
 
pii merge(pii lf, pii rg){
	pii r = {max(lf.first, rg.first), max(lf.second, rg.second)};
 
	return r;
}
 
pii merge2(pii lf, pii rg){
	pii r = {max(lf.first, rg.first), max(lf.second, rg.second)};
 
	return r;
}
 
void build1(int node, int i, int j){
	if(i == j){
		if(i != 1){
			segtree1[node] = {i, -1};
		}else{
			segtree1[node] = {i, i};//pivot is black and white
		}
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;
 
		build1(lf, i, mid);
		build1(rg, mid + 1, j);
 
		segtree1[node] = merge(segtree1[lf], segtree1[rg]);
	}
}
 
pii query1(int node, int i, int j, int l, int r){
	if(i > r || j < l){
		return {-1, -1};
	}
 
	if(l <= i && j <= r){
		return segtree1[node];
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;
 
		return merge(
			query1(lf, i, mid, l, r),
			query1(rg, mid + 1, j, l, r)
		);
	}
}
 
void update1(int node, int i, int j, int pos){
	if(i > pos || j < pos) return;
 
	if(i == pos && j == pos){
		swap(segtree1[node].first, segtree1[node].second);
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;
 
		update1(lf, i, mid, pos);
		update1(rg, mid + 1, j, pos);
 
		segtree1[node] = merge(segtree1[lf], segtree1[rg]);
	}
}
 
void build2(int node, int i, int j){
	if(i == j){
		if(i != 1){
			segtree2[node] = {sz[idx[i]], 0};
		}else{
			segtree2[node] = {sz[0], 0};//pivot
		}
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;
 
		build2(lf, i, mid);
		build2(rg, mid + 1, j);
 
		segtree2[node] = merge2(segtree2[lf], segtree2[rg]);
	}
}
 
pii add(pii a, pii b){
	return {a.first + b.first, a.second + b.second};
}
 
void shift(int node){
	int lf = node << 1;
	int rg = lf + 1;
 
	segtree2[node].first += lazzy[node].first;
	segtree2[node].second += lazzy[node].second;
 
	lazzy[lf] = add(lazzy[lf], lazzy[node]);
	lazzy[rg] = add(lazzy[rg], lazzy[node]);
 
	lazzy[node] = {0, 0};
}
 
pii query2(int node, int i, int j, int l, int r){
	if(lazzy[node].first || lazzy[node].second){
		shift(node);
	}
 
	if(j < l || i > r){
		return {0, 0};
	}
 
	if(l <= i && j <= r){
		return segtree2[node];
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;
 
		return merge2(query2(lf, i, mid, l, r), query2(rg, mid + 1, j, l, r));
	}
}
 
void update2(int node, int i, int j, int l, int r, pii updt){
	if(lazzy[node].first || lazzy[node].second){
		shift(node);
	}
 
	if(j < l || i > r){
		return;
	}
 
	if(l <= i && j <= r){
		lazzy[node] = updt;
 
		shift(node);
	}else{
		int mid = (i + j) >> 1;
		int lf = node << 1;
		int rg = lf + 1;
 
		update2(lf, i, mid, l, r, updt);
		update2(rg, mid + 1, j, l, r, updt);
 
		segtree2[node] = merge2(segtree2[lf], segtree2[rg]);
	}
}
 
void init(){
	parent = vector< int >(N, -1);
	depth = vector< int >(N);
	heavy = vector< int >(N, -1);
	head = vector< int >(N);
	pos = vector< int >(N);
 
	cur_pos = 1;
	dfs(pivot);
 
	decompose(pivot, pivot);
	build1(1, 1, n);
	build2(1, 1, n);
}
 
int query(int a){
	int st = cor[a];
	int best = 0;
 
	while(true){
	    if(cor[a] != st) return best;
	    
		pii r = query1(1, 1, n, pos[head[a]], pos[a]);
		// printf("%d, %d\n", r.first, r.second);
		if(st == 0){
			if(r.second != -1){
				// printf("s: %d\n", idx[r.second]);
				// printf("1.%d->%d\n", idx[r.second], idx[r.second]);
				while(best > query2(1, 1, n, r.second + 1, r.second + 1).first);
				return query2(1, 1, n, r.second + 1, r.second + 1).first;
			}else{
			    // printf("2.%d->%d %d\n", head[a], a, query2(1, 1, n, pos[a], pos[head[a]]).first);
				while(query2(1, 1, n, pos[head[a]], pos[a]).first < best);
				best = max(best, query2(1, 1, n, pos[head[a]], pos[a]).first);
			}
		}else{
			if(r.first != -1){
			    // printf("3.%d->%d %d\n", idx[r.first], idx[r.first + 1], best);
				while(best > query2(1, 1, n, r.first + 1, r.first + 1).second);
				return query2(1, 1, n, r.first + 1, r.first + 1).second;
			}else{
			    // printf("4.%d->%d\n", head[a], a);
			    while(best > query2(1, 1, n, pos[head[a]], pos[a]).second);
				best = max(best, query2(1, 1, n, pos[head[a]], pos[a]).second);
			}
		}
 
		a = parent[head[a]];
	}
 
	return best;
}
 
void update(int a){
	int st = cor[a];
	pii insz;
 
	update1(1, 1, n, pos[a]);
 
	insz = query2(1, 1, n, pos[a], pos[a]);
 
	if(st == 0){
		update2(1, 1, n, pos[a], pos[a], {-1, 1});
		// printf("updt: %d, %d (%d)\n", insz.first - 1, insz.second + 1, a);
	}else{
		update2(1, 1, n, pos[a], pos[a], {1, -1});
	}
 
	cor[a] = !cor[a];
 	
 	a = parent[a];
 	int tmp = a;
 
	while(true){
		pii r = query1(1, 1, n, pos[head[a]], pos[a]);
 
		if(st == 1){
			if(r.second != -1){
				update2(1, 1, n, r.second, pos[a], {insz.first + 1, 0});
				break;
			}else{
				update2(1, 1, n, pos[head[a]], pos[a], {insz.first + 1, 0});
			}
		}else{
			if(r.first != -1){
				update2(1, 1, n, r.first, pos[a], {0, insz.second + 1});
				break;
			}else{
				update2(1, 1, n, pos[head[a]], pos[a], {0, insz.second + 1});
			}
		}
 
		a = parent[head[a]];
	}
	
	a = tmp;
	
	while(true){
		pii r = query1(1, 1, n, pos[head[a]], pos[a]);
 
		if(st == 0){
			if(r.second != -1){
				update2(1, 1, n, r.second, pos[a], {-insz.first, 0});
				break;
			}else{
				update2(1, 1, n, pos[head[a]], pos[a], {-insz.first, 0});
			}
		}else{
			if(r.first != -1){
				update2(1, 1, n, r.first, pos[a], {0, -insz.second});
				break;
			}else{
				update2(1, 1, n, pos[head[a]], pos[a], {0, -insz.second});
			}
		}
 
		a = parent[head[a]];
	}
}
 
int main(){
	scanf("%d", &n);
 
	for(int i = 1 ; i < n ; i++){
		int ui, vi;
 
		scanf("%d %d", &ui, &vi);
 
		ui--, vi--;
		adj[ui].push_back(vi);
		adj[vi].push_back(ui);
	}
 
	adj[pivot].push_back(0);
	n++;
 
	init();
 
	int q;
 
	scanf("%d", &q);
 
	while(q--){
		int op, u;
 
		scanf("%d %d", &op, &u);
 
		u--;
 
		if(op == 0){
			printf("%d\n", query(u));
		}else{
			update(u);
		}
	}
 
	return 0;
}   