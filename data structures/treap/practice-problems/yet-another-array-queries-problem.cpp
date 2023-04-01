// solution for https://codeforces.com/contest/863/problem/D

#include<bits/stdc++.h>

#define int long long

const int N=1e6+10;

using namespace std;

int n,q,m,x,y,a[N];

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int getRand(int l,int r){
    uniform_int_distribution < int > uid(l,r);
    return uid(rng);
}

struct item{
    int priority, value, sz=1, toAdd=0, sum, assignment=-1, rev=0;
    item *left = nullptr ,*right = nullptr;
    explicit item(int value):value(value){
        priority = getRand(0,INT_MAX);
        sum=value;
    }
};
class treap{
private:
    item *root = nullptr;
    int size(item* node){
        return (node==nullptr) ? 0 : node->sz;
    }
    void recalc(item* node){
        node->sz=1+size(node->left)+size(node->right);
        node->sum=node->value+getSum(node->left)+getSum(node->right);
    }
    int getSum(item *node){
        return (node==nullptr)?0:node->sum;
    }
    void pushAdd(item* parent,item *node){
        if (node==nullptr)return;
        node->toAdd += parent->toAdd;
    }
    void pushAssign(item* parent,item *node){
        if (node==nullptr)return;
        node->assignment = parent->assignment;
        node->toAdd = 0;
    }
    void pushRev(item* parent,item* node){
        if (node==nullptr)return;
        node->rev ^= parent->rev;
    }
    void pushHelper(item *node){
        if (node==nullptr)return;
        if (node->assignment != -1){
            node->sum = node->assignment*node->sz;
            node->value = node->assignment;
            node->toAdd = 0;
            pushAssign(node,node->left), pushAssign(node,node->right);
            node->assignment = -1;
        }
    }
    void prop(item* node){
        if (node == nullptr)return;
        pushHelper(node);
        if (node->toAdd != 0){
            node->sum += (node->toAdd*node->sz);
            node->value += (node->toAdd);
            pushHelper(node->left), pushHelper(node->right);
            pushAdd(node,node->left), pushAdd(node,node->right);
            node->toAdd = 0;
        }

        if (node->rev == 1){
            swap(node->left,node->right);
            pushRev(node,node->left), pushRev(node,node->right);
            node->rev=0;
        }
    }
    void propAll(item *node){
        if (node==nullptr)return;
        prop(node);
        prop(node->left);
        prop(node->right);
    }
    vector < item* > split(item* node,int idx){
        if (node == nullptr){
            return {nullptr,nullptr};
        }
        propAll(node);
        if (size(node->left)>=idx){
            vector < item* > ret = split(node->left,idx);
            node->left = ret[1];
            recalc(node);
            return {ret[0],node};
        }
        else {
            vector < item* > ret = split(node->right,idx-size(node->left)-1);
            node->right = ret[0];
            recalc(node);
            return {node,ret[1]};
        }
    }
    item* merge(item* L,item* R){
        propAll(L);
        propAll(R);
        if (L==nullptr)return R;
        if (R==nullptr)return L;
        if (L->priority > R->priority){
            L->right = merge(L->right,R);
            recalc(L);
            return L;
        }
        else {
            R->left = merge(L,R->left);
            recalc(R);
            return R;
        }
    }
    int getSum(item* node,int l,int r){
        vector < item* > part1 = split(node,l);
        vector < item* > part2 = split(part1[1],r-l+1);
        int ret = (part2[0]==nullptr)? 0 : part2[0]->sum;
        root=merge(part1[0],merge(part2[0],part2[1]));
        return ret;
    }
    void segmentOperator(int l,int r,int type,int val=0){
        vector < item* > part1 = split(root, l);
        vector < item* > part2 = split(part1[1],r-l+1);
        if (type == 0) // add
            part2[0]->toAdd += val;
        else if (type == 1) // assign
            part2[0]->assignment = val;
        else if (type == 2) // reverse
            part2[0]->rev ^= 1;
        root = merge(part1[0],merge(part2[0],part2[1]));
    }
public:
    int size(){
        return size(root);
    }
    int operator[](int idx){
        return getSum(root,idx,idx);
    }
    int getSum(int l,int r){
        return getSum(root,l,r);
    }
    void add(int l,int r,int val){
        segmentOperator(l,r,0,val);
    }
    void assign(int l,int r,int val){
        segmentOperator(l,r,1,val);
    }
    void reverse(int l,int r){
        segmentOperator(l,r,2);
    }
    void cycle_shift(int l,int r){
        vector < item* > part1 = split(root, l);
        vector < item* > part2 = split(part1[1],r-l+1);
        vector < item* > part3 = split(part2[0],r-l);
        root = merge(part1[0],merge(part3[1],merge(part3[0],part2[1])));
    }
    void append(int value){
        root = merge(root,new item(value));
    }
    void insert(int position,int value){
        vector < item* > part1 = split(root,position);
        part1[0] = merge(part1[0],new item(value));
        root=merge(part1[0],part1[1]);
    }
};


int32_t main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin>>n>>q>>m;
    treap t;
    for (int i=0;i<n;i++){
        cin>>a[i];
        t.append(a[i]);
    }
    while (q--){
        cin>>x;
        if (x==1){
            cin>>x>>y;
            t.cycle_shift(x-1,y-1);
        }
        else if (x==2){
            cin>>x>>y;
            t.reverse(x-1,y-1);
        }
    }
    for (int i=0;i<m;i++){
        cin>>x;
        cout<<t[x-1]<<" ";
    }
    return 0;
}
 