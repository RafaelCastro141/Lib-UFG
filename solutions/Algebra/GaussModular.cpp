#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long   ll;
 
const int N = 102;
int n, m;
ll k;
ll a[N][N];
const ll mod = 1e9 + 7;
vector< int > adj[N];
 
void swap_lines(int i, int j){
    for(int c = 0 ; c < n ; c++){
        swap(a[i][c], a[j][c]);
    }
}
 
ll fast_pow(ll base, ll expo, ll mod){
    //assert(expo == mod - 2LL);
    ll r = 1LL;
    ll carry = base;
    //cout << base << " ^ " << expo << " % " << mod ;
   
    while(expo){
        if(expo & 1){
            r = (r * carry) % mod;
        }
       
        expo >>= 1;
        carry = (carry * carry) % mod;
    }
   
    //cout << " = " << r << endl;
   
    return r;
}
 
int gauss(){
    int r = 0;
    int last_choosed = -1;
    //show_matrix();
   
    for(int c = 0 ; c < n ; c++){
        int j = r;
       
        for(int i = r ; i < n ; i++){
            //assert(a[i][c] >= 0LL && a[i][c] < k);
           
            if(a[i][c]){
                j = i;
                break;
            }
        }
       
        if(!a[j][c]){
            continue;
        }
       
        //cout << "escolhi a " << j << endl;
       
        swap_lines(r, j);
       
        ll inv = fast_pow(a[r][c], k - 2, k);
       
        for(int col = 0 ; col < n ; col++){
            a[r][col] = (a[r][col] * inv) % k;
        }
       
        for(int i = 0 ; i < n ; i++){
            if(!a[i][c] || i == r){
                continue;
            }
           
            ll t = a[i][c];
           
            for(int j = 0 ; j < n ; j++){
                a[i][j] = ((a[i][j] - t * a[r][j]) % k + k) % k;
            }
        }
       
        //show_matrix();
        r++;
    }
   
    return r;
}
 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
   
    int t;
    int test = 1;
   
    cin >> t;
   
    while(t--){
        cin >> n >> m >> k;
       
        for(int i = 0 ; i < m ; i++){
            int ui, vi;
           
            cin >> ui >> vi;
           
            ui--, vi--;
           
            adj[ui].push_back(vi);
            adj[vi].push_back(ui);
        }
       
        //for each i (1...n) we need to hold the following condition:
        //color[i] - sum(color[adj[i]]) == 0 mod k
       
        for(int i = 0 ; i < n ; i++){
            a[i][i] = 1;
           
            for(int j = 0 ; j < adj[i].size() ; j++){
                a[i][adj[i][j]] = k - 1;//-1 mod k
            }
           
            a[i][n] = 0;
        }
       
        int r = gauss();
       
        cout << "Case " << test++ << ": " << fast_pow(k, n - r, mod) << endl;
        //cout << "rank = " << r << endl;
       
        for(int i = 0 ; i < n ; i++){
            adj[i].clear();
        }
       
        memset(a, 0LL, sizeof a);
    }
 
    return 0;
}