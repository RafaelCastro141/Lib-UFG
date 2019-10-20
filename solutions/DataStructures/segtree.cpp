#define lef(x) (x<<1)
#define rig(x) (lef(x)+1)

int tree[4*N];
void build(int p,int tl,int tr){
	if(tl==tr)
		tree[p]=a[tl];
	else{
		int mid=(tl+tr)/2;
		build(lef(p),tl,mid);
		build(rig(p),mid+1,tr);
		tree[p]=max(tree[lef(p)],tree[rig(p)]);
	}
}


int query(int p,int l,int r,int tl,int tr){
	if(l>r)
		return 0;
	if(tl==l && tr==r)
		return tree[p];
	int mid=(tl+tr)/2;
	return max(query(lef(p),l,min(r,mid),tl,mid),query(rig(p),max(l,mid+1),r,mid+1,tr));

}

void update(int p,ll k,int x,int tl,int tr){
	if(tl==tr){
		// cout<<tl<<" alo"<<k<<"\n";
		tree[p]=k;
	}
	else{
		int mid=(tl+tr)/2;
		if(mid>=x){
			update(lef(p),k,x,tl,mid);
		}else
			update(rig(p),k,x,mid+1,tr);
		tree[p]=max(tree[lef(p)],tree[rig(p)]);
	}
}
