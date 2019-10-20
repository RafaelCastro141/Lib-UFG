#include<bits/stdc++.h>
using namespace std;
#define ll long long 
#define pb push_back
#define sd second
#define ft first
const int N=100100;
int pai[N],rk[N],que[N],lo[N],hi[N];
vector<int> lista[N],mid[N];

	vector<pair<pair<int,int>,int>> v;




void init(){
	for(int i=0;i<N;i++){
		pai[i]=i,rk[i]=1;
	}
}

int find(int x){
	if(x==pai[x])
		return x;
	return pai[x]=find(pai[x]);
}



void join(int a,int b){
	a=find(a);
	b=find(b);
	if(a!=b){
		if(rk[a]<rk[b])
			swap(a,b);
		pai[b]=a;
		rk[a]+=rk[b];
	}
}

vector<pair<int,int>> p;
int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	int n,m;
	int q,a,b,z;
	cin>>n>>m;
	int t=m;
	while(t--){
		cin>>a>>b;
		p.pb({a,b});
	}

	cin>>q;
	for(int i=0;i<q;i++){
		cin>>a>>b>>z;
		lo[i]=0,hi[i]=m;
		v.pb({{a,b},z});
	}
	bool need =true;
	while(need){
		for(int i=0;i<m;i++)
			mid[i].clear();
		for (int i = 0; i < q; ++i)
		{
			if(lo[i]<=hi[i])
			{
				mid[(lo[i]+hi[i])/2].pb(i);
			}
		}
		init();
		need=false;
		for (int i = 0; i < m; ++i)
		{
			join(p[i].ft,p[i].sd);
			for(auto at:mid[i])
			{
				need=true;
				int val;
				if(find(v[at].ft.ft)==find(v[at].ft.sd))
				{
					val=rk[find(v[at].ft.ft)];
				}else{
					val=rk[find(v[at].ft.ft)]+rk[find(v[at].ft.sd)];
				}
				if(val>=v[at].sd)
				{	
					hi[at]=i-1;
					que[at]=i;
				}else{
					lo[at]=i+1;
				}
			}
		}

	}
	for (int i = 0; i < q; ++i)
	{
		cout<<que[i]+1<<"\n";
	}

}