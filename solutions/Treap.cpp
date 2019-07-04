#define tipo int
typedef struct node{
    int p,cnt;
    tipo value;
    bool rev;
    struct node * l;
    struct node * r;
    node() { }
    node(tipo value) : value(value), p(rand()<<15+rand()),cnt(1),rev(false), l(NULL), r(NULL) { }

}Node;

int cnt (Node * t) {
    return t ? t->cnt : 0;
}

void upd_cnt (Node * t) {
    if (t){
        
        t->cnt = 1 + cnt(t->l) + cnt (t->r);
    } 
}

void push (Node * it) {
    if (it && it->rev) {
        it->rev = false;
        swap (it->l, it->r);
        if (it->l)  it->l->rev ^= true;
        if (it->r)  it->r->rev ^= true;
        //upd_cnt(it);
    }
}



void split(Node * t, Node * &l, Node * &r,int key,int add = 0){
    
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

void merge(Node* &t,Node* l, Node* r){
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

void insert(Node* &t,Node* n,int key){  
    Node *t1,*t2;
    split(t,t1,t2,key);
    // cout<<"oi\n";
    merge(t1,t1,n);
    merge(t,t1,t2);
}

void reverse (Node * t, int l, int r) {
    if(l>=r)
        return;
    Node * t1, * t2, * t3;
    split (t, t1, t2, l);
    split (t2, t2, t3, r-l+1);
    t2->rev ^= true;
    merge (t, t1, t2);
    merge (t, t, t3);
}
void output (Node* t) {
    if (!t)  return;
    // push (t);
    output (t->l);
    cout<<t->value<<" ";
    output (t->r);
}
void clr (Node* &t) {
    if (!t)  return;
    clr (t->l);
    clr (t->r);
    delete t;
    t=NULL;
    // cout<<t<<"\n";
}