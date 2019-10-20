#include <bits/stdc++.h>

using namespace std;

const int N = int(1e5);
vector< int > adj[N], rev[N];
int comp[N];
bool marked1[N];
bool marked2[N];
vector< int > topological_sort;

int neg(int a){
	return a ^ 1;	
}

void dfs1(int u){
	marked1[u] = true;
	
	for(int i = 0 ; i < adj[u].size() ; i++){
		int v = adj[u][i];
		
		if(!marked1[v]){
			dfs1(v);
		}
	}
	
	topological_sort.push_back(u);
}

void dfs2(int u, int comp_number){
	marked2[u] = true;
	comp[u] = comp_number;
	
	for(int i = 0 ; i < rev[u].size() ; i++){
		int v = rev[u][i];
		
		if(!marked2[v]){
			dfs2(v, comp_number);
		}
	}
}

bool run(int n){
	memset(marked1, false, sizeof marked1);
	memset(marked2, false, sizeof marked2);
	memset(comp, 0, sizeof comp);

	for(int i = 1 ; i <= n + 1 ; i++){
		int v = 2 * i;
		
		if(!marked1[v]){
			dfs1(v);
		}
		
		if(!marked1[neg(v)]){
			dfs1(neg(v));
		}
	}
	
	int comp_number = 1;
	
	for(int i = int(topological_sort.size()) - 1 ; i >= 0 ; i--){	
		int v = topological_sort[i];
		
		if(!marked2[v]){
			dfs2(v, comp_number);
			comp_number++;
		}
	}
	
	topological_sort.clear();
}

class TSAT{
	private:
		int n;
		int sent;
	public:
		void set_n(int _n){
			n = _n;
			sent = 2 * (n + 1);
		}
		
		void add_clause(int a, int b, int nega, int negb){
			a = 2 * a + int(nega);
			b = 2 * b + int(negb);
			
			adj[neg(a)].push_back(b);
			adj[neg(b)].push_back(a);
			
			rev[b].push_back(neg(a));
			rev[a].push_back(neg(b));
		}
		
		bool is_satisfatible(){
			run(n);
			
			for(int i = 1 ; i <= n + 1 ; i++){
				int v = 2 * i;
				
				if(comp[v] == comp[neg(v)]){
					return false;
				}
			}
			
			return true;
		}
		
		void clear(){
			for(int i = 1 ; i <= n + 1 ; i++){
				int v = 2 * i;
				
				adj[v].clear();
				adj[neg(v)].clear();
				
				rev[v].clear();
				rev[neg(v)].clear();		
			}
		}
};

int main(){
	int c, p;
	TSAT *sat;
	
	sat = new TSAT();
	
	while(scanf("%d %d", &c, &p) != EOF){
		if(c == p && c == 0){
			break;
		}
		
		sat->set_n(p);
		
		for(int i = 0 ; i < c ; i++){
			int x, y, s, t;
			
			scanf("%d %d %d %d", &x, &y, &s, &t);
			
			if(x != 0 || y != 0){
				if(x == 0) x = y;
				if(y == 0) y = x;

				sat->add_clause(x, y, 0, 0);
			}
			
			if(s != 0 || t != 0){
				if(s == 0) s = t;
				if(t == 0) t = s;

				sat->add_clause(s, t, 1, 1);
			}
		}
		
		if(sat->is_satisfatible()){
			printf("yes\n");
		}else{
			printf("no\n");
		}
		
		sat->clear();
	}
	
	return 0;
}
