int st[4*N][K],lo[N];
int query(int l,int r){
	long long sum = 0;
	for (int j = K; j >= 0; j--) {
	    if ((1 << j) <= R - L + 1) {
	        sum += st[L][j];
	        L += 1 << j;
	    }
	}
}

void build(){
	lo[1]=0;
	for(int i=2;i<N;i++)
		lo[i]=lo[i/2]+1;
	for(int i=0;i<n;i++)
		st[i][0]=a[i];
	for(int j=1;j<K;j++){
		for(int i=0;i+(1<<(j))<=n;i++){
			st[i][j]=max(st[i][j-1],st[i+(1<<(j-1))][j-1]);
		}
	}
}


int queryidempo(int l,int r){
	int at=lo[r-l+1];
	return max(st[l][at],st[r-(1<<(at))+1][at]);
}

int query(int l,int r){
	long long sum = 0;
	for (int j = K; j >= 0; j--) {
	    if ((1 << j) <= R - L + 1) {
	        sum += st[L][j];
	        L += 1 << j;
	    }
	}
}