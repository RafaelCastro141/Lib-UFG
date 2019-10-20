#include <bits/stdc++.h>
 
using namespace std;
#define MAX 100010
typedef long long   ll;
typedef pair< ll, int >    pii;
 
struct event 
{
    int ind; // Index of rectangle in rects
    bool type; // Type of event: 0 = Lower-left ; 1 = Upper-right
    event() {};
    event(int ind, int type) : ind(ind), type(type) {};
};
 
struct point 
{
    ll x, y;
};
point rects [MAX][12]; // Each rectangle consists of 2 points: [0] = lower-left ; [1] = upper-right
bool compare_x(event a, event b) { return rects[a.ind][a.type].x<rects[b.ind][b.type].x; }
bool compare_y(event a, event b) { return rects[a.ind][a.type].y<rects[b.ind][b.type].y; }
 
int mark[MAX][2];
const int N = int(1e6), BOUND_Y = int(1e5 + 100);
pii segtree[N << 3];
int lazy[N<<3];
 
int get(int node, int sz){
    if(segtree[node].first){
        assert(segtree[node].first > 0);
        
        return sz;
    }    
    
    return segtree[node].second;
}
 
void update(int node, int i, int j, int l, int r, int val){
    if(i > r || j < l){
        return;
    }
    
    if(i >= l && j <= r){
        segtree[node].first += val;
    }else{
        int lf = node << 1;
        int rg = lf + 1;
        int mid = (i + j) >> 1;
        
        update(lf, i, mid, l, r, val);
        update(rg, mid + 1, j, l, r, val);
        
        segtree[node].second = get(lf, mid - i + 1) + get(rg, j - (mid + 1) + 1);
    }
}
 
ll query(int node, int i, int j, int l, int r){
    if(i > r || j < l){
        return 0LL;
    }
    
    if(i >= l && j <= r){
        return get(node, j - i + 1LL);   
    }else{
        int lf = node << 1;
        int rg = lf + 1;
        int mid = (i + j) >> 1;
        
        return query(lf, i, mid, l, r) + query(rg, mid + 1, j, l, r);
    }
}
 
ll union_area(event events_v[],event events_h[],int n,int e)
{
   //n is the number of rectangles, e=2*n , e is the number of points (each rectangle has two points as described in declaration of rects)
    ll area=0;
    
    sort(events_v, events_v+e, compare_x);  //Pre-sort of vertical edges
    sort(events_h, events_h+e, compare_y); // Pre-sort set of horizontal edges
    
    update(1, 1, BOUND_Y, rects[events_v[0].ind][0].y + 1, rects[events_v[0].ind][1].y, 1);
   // cout << "adding " << rects[events_v[0].ind][0].y << " --- " << rects[events_v[0].ind][1].y << endl;
    
    for (int i=1;i<e;++i) 
    { // Vertical sweep line
        event c = events_v[i];
        int cnt = 0; // Counter to indicate how many rectangles are currently overlapping
        // Delta_x: Distance between current sweep line and previous sweep line
        ll delta_x = rects[c.ind][c.type].x - rects[events_v[i - 1].ind][events_v[i - 1].type].x;
        ll begin_y;
        
        if (delta_x == 0){
            if(c.type == 0){
        //        cout << "adding " << rects[c.ind][0].y << " --- " << rects[c.ind][1].y << endl;
            
                update(1, 1, BOUND_Y, rects[c.ind][0].y + 1, rects[c.ind][1].y, 1);
            }else{
     //           cout << "removing " << rects[c.ind][0].y << " --- " << rects[c.ind][1].y << endl;
 
                update(1, 1, BOUND_Y, rects[c.ind][0].y + 1, rects[c.ind][1].y, -1);
            }
            
            continue;
        }
        
        ll delta_y = query(1, 1, BOUND_Y, 1, BOUND_Y);//length of the vertical sweep line cut by rectangles
    //    cout << "we have " << delta_y << " x " << delta_x << endl;
        area += delta_x * delta_y;
        
        if(c.type == 0){
    //        cout << "adding " << rects[c.ind][0].y << " --- " << rects[c.ind][1].y << endl;
            
            update(1, 1, BOUND_Y, rects[c.ind][0].y + 1, rects[c.ind][1].y, 1);
        }else{
   //         cout << "removing " << rects[c.ind][0].y << " --- " << rects[c.ind][1].y << endl;
            
            update(1, 1, BOUND_Y, rects[c.ind][0].y + 1, rects[c.ind][1].y, -1);
        }
        //in_set[c.ind] = (c.type==0);//If it is a left edge, the rectangle is in the active set else not
    }
    
    return area;
}
 
event events_v[2 * MAX], events_h[2 * MAX];
 
int main(){
    int n;
    
    cin >> n;
    
    for(int i = 0 ; i < n ; i++){
        cin >> rects[i][0].x >> rects[i][0].y;
        cin >> rects[i][1].x >> rects[i][1].y;
        
        events_v[i * 2].ind = i;
        events_v[i * 2].type = 0;
        events_v[i * 2 + 1].ind = i;
        events_v[i * 2 + 1].type = 1;
        
        events_h[i * 2].ind = i;
        events_h[i * 2].type = 0;
        events_h[i * 2 + 1].ind = i;
        events_h[i * 2 + 1].type = 1;
    }
    
    cout << union_area(events_v, events_h, n, 2 * n) << endl;
    return 0;
}