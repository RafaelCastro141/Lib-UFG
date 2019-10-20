ll tree[4*N];
ll lazy[4*N];

void push(ll p,ll l,ll r){
		ll mid=(r+l)/2LL;
		lazy[lef(p)]+=lazy[p];
		lazy[rig(p)]+=lazy[p];
		tree[lef(p)]+=max((mid-l+1LL),1LL)*lazy[p];
		tree[rig(p)]+=max((r-mid),1LL)*lazy[p];
		lazy[p]=0LL;
	
}




void update(ll p,ll l,ll r,ll x,ll tl,ll tr){
	if(l>r)
		return;
	if(l==tl && r==tr){
		lazy[p]+=x;
		tree[p]+=(ll)(tr-tl+1LL)*x;
	}else{
		push(p,tl,tr);
		ll mid=(tl+tr)/2;
		update(lef(p),l,min(r,mid),x,tl,mid);
		update(rig(p),max(l,mid+1LL),r,x,mid+1LL,tr);
		tree[p]=tree[lef(p)]+tree[rig(p)];
	}
}

ll query(ll p,ll l,ll r,ll tl,ll tr){
	if(l>r)
		return 0LL;
	if(l<=tl && r>=tr){
		// cout<<tl<<" "<<tr<<" "<<tree[p]<<"\n";
		return tree[p];
	}
	push(p,tl,tr);
	ll mid=(tl+tr)/2LL;

	return query(lef(p),l,min(r,mid),tl,mid)+
		query(rig(p),max(l,mid+1LL),r,mid+1LL,tr);
}