#include <bits/stdc++.h>

using namespace std;

typedef pair< int, int >	pii;
const int N = 40;

class Graph{//dont works with multiedges, if u->v exists then v->u dont exists (loops u->u cant exists)
public:
	int sink, source;
	int n, tot;
	int c[N][N];
	int adj[N][N];
	int sadj[N][N];
	int dis[N];
	int neg_cycle[N];
	int father[N];
	int marked[N];
	vector< pii > edges;
	vector< int > graph[N];
	int pos = 0;
	int runt = 1;
	const int inf = int(1e9);

	Graph(int n){
		this->n = n;
		source = n + 1;
		sink = n + 2;
		tot = n + 3;
		memset(adj, 0, sizeof adj);
		memset(c, 0, sizeof c);
	}

	void clear(){
		source = n + 1;
		sink = n + 2;
		tot = n + 3;
		memset(sadj, 0, sizeof sadj);
		memset(adj, 0, sizeof adj);
		memset(c, 0, sizeof c);

		for(int i = 0 ; i < tot ; i++){
			graph[i].clear();
		}

		edges.clear();
	}

	Graph(){

	}

	void add_edge(int u, int v, int weight, int cost = 0){//edge u->v with weight and cost
		adj[u][v] = weight;
		c[u][v] = cost;
		c[v][u] = -cost;
		sadj[u][v] = weight;

		edges.push_back({u, v});
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	bool bfs(){
		memset(dis, -1, sizeof dis);

		queue< int > q;
		q.push(source);

		dis[source] = 0;
		father[source] = source;

		while(!q.empty()){
			int u = q.front();
			q.pop();

			for(int i: graph[u]){
				if(adj[u][i] && dis[i] == -1){
					dis[i] = dis[u] + 1;
					father[i] = u;

					q.push(i);
				}
			}
		}

		return dis[sink] != -1;
	}

	int ford_fukerson(){
		int flow = 0;

		while(bfs()){
			int mn_edge = inf;
			int cur = sink;

			while(cur != source){
				mn_edge = min(mn_edge, adj[father[cur]][cur]);
				cur = father[cur];
			}

			cur = sink;

			while(cur != source){
				adj[father[cur]][cur] -= mn_edge;
				adj[cur][father[cur]] += mn_edge;
				cur = father[cur];
			}

			flow += mn_edge;
		}

		return flow;
	}

	bool belman_ford(){
		for(int i = 0 ; i < tot ; i++){
			dis[i] = 0;
			father[i] = -1;
		}

		int x = -1;

		for(int i = 0 ; i < tot ; i++){
			x = -1;

			for(int j = 0 ; j < edges.size() ; j++){
				int u = edges[j].first;
				int v = edges[j].second;

				if(dis[u] + c[u][v] < dis[v] && adj[u][v]){
					father[v] = u;
					dis[v] = dis[u] + c[u][v];
					x = v;
				}

				swap(u, v);

				if(dis[u] + c[u][v] < dis[v] && adj[u][v]){
					father[v] = u;
					dis[v] = dis[u] + c[u][v];
					x = v;
				}
			}
		}

 		if(x == -1){
			return false;
		}else{
			for(int i = 0 ; i < tot ; i++){
				x = father[x];
			}

			pos = 0;

			for(int v = x;; v = father[v]){
				neg_cycle[pos++] = v;

				if(v == x && pos > 1){
					break;
				}
			}

			return true;
		}
	}

	pii cycle_canceling(){//returns a tuple < maxflow, mincost >
		int flow = ford_fukerson();

		while(belman_ford()){
			int mn_edge = inf;

			for(int i = 0 ; i < pos - 1 ; i++){
				mn_edge = min(mn_edge, adj[neg_cycle[i + 1]][neg_cycle[i]]);
			}

			for(int i = 0 ; i < pos - 1 ; i++){
				adj[neg_cycle[i + 1]][neg_cycle[i]] -= mn_edge;
				adj[neg_cycle[i]][neg_cycle[i + 1]] += mn_edge;
			}
		}

		int cost = 0;

		for(int i = 0 ; i < edges.size() ; i++){
			int u = edges[i].first;
			int v = edges[i].second;

			cost += (sadj[u][v] - adj[u][v]) * c[u][v];
		}

		return {flow, cost};
	}
};


int main(){
	int n;

	scanf("%d", &n);

	Graph *graph = new Graph(2 * n);

	for(int j = 0 ; j < n ; j++){
		graph->add_edge(graph->source, j, 1, 0);
		graph->add_edge(j + n, graph->sink, 1, 0);
	}

	for(int i = 0 ; i < n ; i++){
		for(int j = 0 ; j < n ; j++){
			int aij;

			scanf("%d", &aij);

			graph->add_edge(i, j + n, 1, aij);
		}
	}

	pii r = graph->cycle_canceling();

	printf("%d\n", r.second);

	return 0;
}