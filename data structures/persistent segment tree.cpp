struct node{
    int sum=0;
    node *l,*r;
    node(int x=0){
        sum=x;
    }
    node(node* l,node *r):l(l),r(r){
        if (l)sum+=l->sum;
        if (r)sum+=r->sum;
    }
};

node* build(int tl,int tr){
    if (tl==tr)return new node(a[tl]);
    int tm=(tl+tr)/2;
    return new node(build(tl,tm),build(tm+1,tr));
}

node* update(node* v,int tl,int tr,int pos,int val){
    if (tl==tr)return new node(val);
    int tm=(tl+tr)/2;
    if (pos<=tm)return new node(update(v->l,tl,tm,pos,val),v->r);
    else return new node(v->l,update(v->r,tm+1,tr,pos,val));
}

int query(node *v,int tl,int tr,int l,int r){
    if (tl>r||tr<l)return 0;
    if (tl>=l&&tr<=r)return v->sum;
    int tm=(tl+tr)/2;
    return query(v->l,tl,tm,l,r)+query(v->r,tm+1,tr,l,r);
}