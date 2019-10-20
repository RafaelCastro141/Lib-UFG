#include <bits/stdc++.h>
 
using namespace std;

typedef pair< int, int >	pii;
typedef double	ld;
typedef long long	ll;
typedef unsigned long long	ull;
typedef pair< ll, ll >	pll;
 
const int N = int(1e5 + 10);
pll rectangle[N][2];
 
struct event{
public:
	int type;
	int idx;
 
	event(){
 
	}
 
	event(int tpe, int id){
		this->type = tpe;
		this->idx = id;
	}
};
 
ll cross(pll a, pll b, pll c){
    ll dx1 = a.first - b.first, dy1 = a.second - b.second;
    ll dx2 = c.first - b.first, dy2 = c.second - b.second;

    return dx1 * dy2 - dx2 * dy1;
}

struct MNode{
public:
	int idx;
 
	MNode(){
 
	}
 
	MNode(int id){
		this->idx = id;
	}
 
	bool operator <(MNode other) const{
        if(rectangle[idx][0].first >= rectangle[other.idx][0].first){
            ll a =  cross(rectangle[other.idx][0], rectangle[idx][0], rectangle[other.idx][1]);

            return cross(rectangle[other.idx][0], rectangle[idx][0], rectangle[other.idx][1]) > 0;
        }else{
            ll a = cross(rectangle[idx][0], rectangle[other.idx][0], rectangle[idx][1]);

            return cross(rectangle[idx][0], rectangle[other.idx][0], rectangle[idx][1]) < 0;
        }
	}
};
 
vector< event > events;
pll balls[N];
int adj[N];
int adj_balls[N];
set< MNode > st;
ll resp[N];
int prio[] = {0, 2, 1};
 
bool compar(event &a, event &b){
	ll xa, xb;
	ll ya, yb;
 
	if(a.type <= 1){
		xa = rectangle[a.idx][a.type].first;
		ya = rectangle[a.idx][a.type].second;
	}else{
		xa = balls[a.idx].first;
		ya = balls[a.idx].second;
	}
 
	if(b.type <= 1){
		xb = rectangle[b.idx][b.type].first;
		yb = rectangle[b.idx][b.type].second;
	}else{
		xb = balls[b.idx].first;
		yb = balls[b.idx].second;
	}
 
	if(xa < xb){
		return true;
	}else if(xa > xb){
		return false;
	}else{
		if(prio[a.type] == prio[b.type]){
            if(a.type == 1){
                return ya > yb;
            }

			return ya < yb;
		}
		
		return prio[a.type] < prio[b.type];
	}
}
 
ll m_point(int u){
	ll mx;
 
	if(rectangle[u][0].second > rectangle[u][1].second){
		mx = rectangle[u][1].first;
	}else{
		mx = rectangle[u][0].first;
	}
 
	return mx;
}
 
bool intersect(int u, int v){
	ll mx = m_point(u);
 
	return rectangle[v][0].first <= mx && mx <= rectangle[v][1].first;
}
 
void line_sweep(){
	sort(events.begin(), events.end(), compar);
 
	for(int i = 0 ; i < events.size() ; i++){
		event u = events[i];
        // printf("%d: %d %d\n", i, u.idx, u.type);

		if(u.type == 2){
			rectangle[N - 1][0] = rectangle[N - 1][1] = balls[u.idx];
 
			if(st.size() == 0) continue;
 
			adj_balls[u.idx] = st.begin()->idx;
		}else{
			if(u.type == 1){
                // printf("procurando!\n");
				auto d = st.find(MNode(u.idx));
                // printf("achado!! (%d, %d, %d)\n", d->idx, u.idx, u.type);

                // for(auto u: st){
                //     printf("%d\n", u.idx);
                // }
                // assert(d != st.end());

                if(rectangle[u.idx][0].second > rectangle[u.idx][1].second){
                    auto nxt = next(d);

                    if(nxt != st.end()){
                        adj[d->idx] = nxt->idx;
                        // printf("%d->%d\n", d->idx, nxt->idx);
                    }
                }
                // printf("limpando!\n");
				st.erase(d);
                // printf("limpei!\n");
			}else{
				st.insert(MNode(u.idx));
				auto d = st.find(MNode(u.idx));
 
				if(rectangle[u.idx][0].second < rectangle[u.idx][1].second){
                    auto nxt = next(d);

                    if(nxt != st.end()){
                        adj[d->idx] = nxt->idx;
                    }
                }
			}
		}
	}
 
	st.clear();
	events.clear();
}
 
int get_resp(int u){
	// cout << "u = " << u << " " << adj[u] << endl;
	if(resp[u] != -1){
		return resp[u];
	}
 
	if(adj[u] == -1){
		return m_point(u);
	}
 
	return resp[u] = get_resp(adj[u]);
}
 
int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
 
	memset(adj_balls, -1, sizeof adj_balls);
	memset(adj, -1, sizeof adj);
	memset(resp, -1, sizeof resp);
 
	int n;
 
	cin >> n;
 
	for(int i = 0 ; i < n ; i++){
		cin >> rectangle[i][0].first >> rectangle[i][0].second >> rectangle[i][1].first >> rectangle[i][1].second;
 
		if(rectangle[i][1].first < rectangle[i][0].first){
			swap(rectangle[i][1], rectangle[i][0]);
		}
	}
 
	int m = 1;
 
	for(int i = 0 ; i < m ; i++){
		cin >> balls[i].first;
 
		balls[i].second = ll(1e9);
	}
 
	for(int i = 0 ; i < n ; i++){
		events.push_back(event(0, i));
		events.push_back(event(1, i));
	}
 
	for(int i = 0 ; i < m ; i++){
		events.push_back(event(2, i));
	}
	// printf("sweep!\n");
 
	line_sweep();
 
	// printf("adone!\n");
	for(int i = 0 ; i < m ; i++){
		if(adj_balls[i] == -1){
			cout << balls[i].first << endl;
		}else{
			cout << get_resp(adj_balls[i]) << endl;
		}
	}
 
	return 0;
}