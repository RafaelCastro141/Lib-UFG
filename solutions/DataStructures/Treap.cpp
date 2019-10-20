#define tipo int
 mt19937 mt_rand(time(0));
struct Node{
    int p,cnt;
    tipo value;
    bool rev;
    struct Node * l;
    struct Node * r;
    node() { }
    node(tipo value) : value(value), p(mt_rand()),cnt(1),rev(false), l(NULL), r(NULL) { }

};
typedef Node * pnode;

int cnt (pnode t) {
    return t ? t->cnt : 0;
}

void upd_cnt (pnode t) {
    if (t){
        
        t->cnt = 1 + cnt(t->l) + cnt (t->r);
    } 
}

void push (pnode it) {
    if (it && it->rev) {
        it->rev = false;
        swap (it->l, it->r);
        if (it->l)  it->l->rev ^= true;
        if (it->r)  it->r->rev ^= true;
        //upd_cnt(it);
    }
}



void split(pnode t, pnode &l, pnode &r,int key,int add = 0){
    
    if(!t){
        return void( l = r = NULL );
    }
    push(t);
    int cur_key=add+cnt(t->l);
    if(key<=cur_key){
        split(t->l,l,t->l,key,add);
        r=t;
    }else{
        split(t->r,t->r,r,key,add+1+cnt(t->l));
        l=t;
    }
    upd_cnt (t);
}

void merge(pnode &t,pnode l, pnode r){
    push(l);
    push(r);
    if (!l || !r)
        t = l ? l : r;
    else if(l->p>r->p){
        merge(l->r,l->r,r),t=l;
    }else{
        merge(r->l,l,r->l),t=r;
    }
    upd_cnt (t);
}

void insert(pnode &t,pnode n,int key){  
    pnode t1,t2;
    split(t,t1,t2,key);
    // cout<<"oi\n";
    merge(t1,t1,n);
    merge(t,t1,t2);
}

void reverse (pnode t, int l, int r) {
    if(l>=r)
        return;
    pnode t1,t2,t3;
    split (t, t1, t2, l);
    split (t2, t2, t3, r-l+1);
    t2->rev ^= true;
    merge (t, t1, t2);
    merge (t, t, t3);
}
void output (pnode t) {
    if (!t)  return;
    // push (t);
    output (t->l);
    cout<<t->value<<" ";
    output (t->r);
}
void clr (pnode &t) {
    if (!t)  return;
    clr (t->l);
    clr (t->r);
    delete t;
    t=NULL;
    // cout<<t<<"\n";
}

void erase(pnode &t,int key)
{
    pnode t1,t2,t3;
    split(t,t1,t2,key-1);
    split(t2,t2,t3,key);
    merge(t,t1,t3);
}