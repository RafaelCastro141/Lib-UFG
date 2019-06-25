#include <bits/stdc++.h>

using namespace std;
typedef pair<int, int> pii;
typedef long double ld;
typedef long long ll;
typedef unsigned long long ull;
typedef pair< ll, ll >  pll;

const int N = int(1e5 + 10);
pll rectangle[N][2];//Line Segments
const int add_seg = 0, remove_seg = 1, add_point = 2;//priority = add_seg, add_point, remove_seg | if you want to change the priority, just modify compar function
pll tmp_p[N];

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

struct fraction{
    public:
    ll num;
    ll dem;

    fraction(ll nm, ll dm){
        this->num = nm;
        this->dem = dm;
    }

    fraction(){
        
    }

    void simplify(){
        ll g = __gcd(this->num, this->dem);

        this->num /= g;
        this->dem /= g;

        if(this->dem < 0LL){//if you dont do this, the operator < wont work
            this->dem *= -1LL;
            this->num *= -1LL;
        }
    }

    bool operator <(fraction &other){
        if(this->dem < 0LL){
            this->dem *= -1LL;
            this->num *= -1LL;
        }

        if(other.dem < 0LL){
            this->dem *= -1LL;
            this->num *= -1LL;
        }

        //printf("%lld < %lld\n",  this->num * other.dem, other.num * this->dem);

        return this->num * other.dem < other.num * this->dem;
    }

    bool operator ==(fraction other){
        if(this->dem < 0LL){
            this->dem *= -1LL;
            this->num *= -1LL;
        }

        if(other.dem < 0LL){
            this->dem *= -1LL;
            this->num *= -1LL;
        }

        //printf("%lld < %lld\n",  this->num * other.dem, other.num * this->dem);

        return this->num * other.dem == other.num * this->dem;
    }
};

bool is_vertical(int idx){
    return rectangle[idx][0].first == rectangle[idx][1].first;
}

fraction dis(int idx, ll mx){
    fraction da;

    if(rectangle[idx][1].first == rectangle[idx][0].first){
        da = fraction( rectangle[idx][0].second, 1LL );
    }else{
        fraction m_angle = fraction((rectangle[idx][1].second - rectangle[idx][0].second) ,
        (rectangle[idx][1].first - rectangle[idx][0].first) );

        da = fraction(m_angle.num * (mx - rectangle[idx][0].first) + m_angle.dem * rectangle[idx][0].second, m_angle.dem);
    }

    assert(da.dem >= 0);

    da.simplify();

    return da;
}

bool var = false;

struct MNode{
    public:
    int idx;

    MNode(){

    }

    MNode(int id){
        this->idx = id;
    }

    bool operator <(MNode other) const{
        fraction da, db;
        
        ll mx = max(rectangle[other.idx][0].first, rectangle[this->idx][0].first);

        da = dis(this->idx, mx);
        db = dis(other.idx, mx);
        
        if(da < db){
            
            return true;
        }else if(da == db){
            //printf("%d, %d\n", this->idx, other.idx);
            if(this->idx == N - 1 || other.idx == N - 1 || is_vertical(other.idx)){
                return false;
            }

            if(is_vertical(this->idx)){
                return true;
            }

            mx = min(rectangle[other.idx][1].first, rectangle[this->idx][1].first);

            da = dis(this->idx, mx);
            db = dis(other.idx, mx);

            if(da < db){
                return true;
            }else if(da == db){
                return this->idx < other.idx;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
};

vector< event > events;
pll balls[N];
set< MNode > st;
int it = 0;

bool compar(event &a, event &b){
    it++;
    // cout << a.idx << " " << a.type << " " << b.idx << " " << b.type << endl;
    int xa, xb;
    int ya, yb;

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
        if(b.type == add_seg){
            return false;
        }else if(a.type == add_seg){
            return true;
        }

        if(b.type == add_point){
            return false;
        }else if(a.type == add_point){
            return true;
        }
        
        return ya < yb;
    }
}

int m_point(int u){
    int mx;

    if(rectangle[u][0].second > rectangle[u][1].second){
        mx = rectangle[u][1].first;
    }else{
        mx = rectangle[u][0].first;
    }

    return mx;
}

bool intersect(int u, int v){
    int mx = m_point(u);

    return rectangle[v][0].first <= mx && mx <= rectangle[v][1].first;
}

bool nao_tem(int idx, set< MNode > st){
    printf("%lu: ", st.size());
    for(auto u: st){
        if(u.idx == idx){
            return false;
        }    
        printf("%d, ", u.idx);
    }
    
    printf("\n");
    
    return true;
}

bool line_sweep(){
    //printf("aqui!!0\n");
  //  fflush(stdout);
    // cout << "tenho " << events.size() << " eventos" << endl;
    sort(events.begin(), events.end(), compar);
   // printf("aqui!!\n");
  //  fflush(stdout);
    // cout << "ja ordenei!" << endl;

    for(int i = 0 ; i < events.size() ; i++){
        event u = events[i];
//printf("size = %lu %d\n\n", st.size(), u.type);
        
        if(u.type == 2){
            //printf("search point %d\n", u.idx + 1);
            rectangle[N - 1][0] = rectangle[N - 1][1] = balls[u.idx];

            auto d = st.lower_bound(MNode(N - 1));

            if(d == st.end() || d == st.begin() || dis(d->idx, balls[u.idx].first) == dis(N - 1, balls[u.idx].first) || is_vertical(d->idx)){
                return false;
            }
        }else{
           // printf("size = %lu\n", st.size());
            if(u.type == remove_seg){
                //printf("remove segment %d\n", u.idx);
                st.erase(MNode(u.idx));
            }else{
                //printf("add segment %d\n", u.idx);
                //printf("add %d\n", u.idx);
                //fflush(stdout);
                st.insert(MNode(u.idx));
            }
        }
    }

    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;

    scanf("%d", &n);

    for(int i = 0 ; i < n ; i++){
        scanf("%lld %lld", &tmp_p[i].first, &tmp_p[i].second);
    }

    for(int i = 0 ; i < n ; i++){
        rectangle[i][0] = tmp_p[i];
        rectangle[i][1] = tmp_p[(i + 1) % n];

        if(rectangle[i][1].first < rectangle[i][0].first){
            swap(rectangle[i][1], rectangle[i][0]);
        }else if(rectangle[i][1].first == rectangle[i][0].first && rectangle[i][1].second > rectangle[i][0].second){
            swap(rectangle[i][1], rectangle[i][0]);
        }
    }

    int m;

    scanf("%d", &m);

    for(int i = 0 ; i < m ; i++){
        scanf("%lld %lld", &balls[i].first, &balls[i].second);
    }       
    // cout << "aqui" << endl;

    for(int i = 0 ; i < n ; i++){
        events.push_back(event(add_seg, i));
        events.push_back(event(remove_seg, i));
    }

    for(int i = 0 ; i < m ; i++){
        events.push_back(event(add_point, i));
    }
    // cout << "rodando o line sweep" << endl;

    if(line_sweep()){
        printf("YES\n");
    }else{
        printf("NO\n");
    }
    
    return 0;
}