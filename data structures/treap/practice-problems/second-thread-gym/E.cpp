// solution for https://codeforces.com/gym/102787/problem/E

#include<bits/stdc++.h>

#define LL long long
#define pb push_back
#define F first
#define S second
#define all(x) x.begin(),x.end()

const double eps=1e-7,PI=3.14159265358979323846;
const int N=1e6+10;

using namespace std;

int n,q,m,k,x,y,a[N],mx=-1,mn=1e9,sum;
string s,s1,s2;
map < int , int > mp;
vector < int > vec;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int getRand(int l,int r){
    uniform_int_distribution < int > uid(l,r);
    return uid(rng);
}

struct item{
    int priority, value, sz=1, toAdd=0, sum, assignment=-1, rev=0, key=0,flip;
    int pref1,pref0,suf1,suf0,mx1,mx0;
    item *left = nullptr ,*right = nullptr;
    item(int value,int key=0):value(value),key(key){
        priority = getRand(0,INT_MAX);
        sum=value;
        pref0=suf0=mx0=(value==0);
        pref1=suf1=mx1=(value==1);
        rev=0;
        flip=0;
    }
};
#define pnn pair < item* , item* >

class treap{
private:
    item *root = nullptr;
    int size(item* &node){
        return (node==nullptr) ? 0 : node->sz;
    }
        int getSuf0(item* &node){
        return (node==nullptr) ? 0 : node->suf0;
    }
    int getPref0(item* &node){
        return (node==nullptr) ? 0 : node->pref0;
    }
    int getSuf1(item* &node){
        return (node==nullptr) ? 0 : node->suf1;
    }
    int getPref1(item* &node){
        return (node==nullptr) ? 0 : node->pref1;
    }
    int getMx0(item* &node){
        return (node==nullptr) ? 0 : node->mx0;
    }
    int getMx1(item* &node){
        return (node==nullptr) ? 0 : node->mx1;
    }
    void recalc(item* &node){
        node->sz=1+size(node->left)+size(node->right);
        node->sum=node->value+getSum(node->left)+getSum(node->right);

        node->pref0 = getPref0(node->left);
        if (getPref0(node->left) == size(node->left) && node->value==0){
            node->pref0 += 1+getPref0(node->right);
        }
        node->suf0 = getSuf0(node->right);
        if (getSuf0(node->right) == size(node->right) && node->value==0){
            node->suf0 += 1+getSuf0(node->left);
        }
        node->mx0 = max({getMx0(node->left),getMx0(node->right),getSuf0(node),getPref0(node),(node->value==0)*(1+getSuf0(node->left)+getPref0(node->right))});

        node->pref1 = getPref1(node->left);
        if (getPref1(node->left) == size(node->left) && node->value==1){
            node->pref1 += 1+getPref1(node->right);
        }
        node->suf1 = getSuf1(node->right);
        if (getSuf1(node->right) == size(node->right) && node->value==1){
            node->suf1 += 1+getSuf1(node->left);
        }
        node->mx1 = max({getMx1(node->left),getMx1(node->right),getSuf1(node),getPref1(node),(node->value==1)*(1+getSuf1(node->left)+getPref1(node->right))});
    }
    int getSum(item* &node){
        return (node==nullptr)?0:node->sum;
    }
    void pushAdd(item* &parent,item* &node){
        if (node==nullptr)return;
        node->toAdd += parent->toAdd;
    }
    void pushAssign(item* &parent,item* &node){
        if (node==nullptr)return;
        node->assignment = parent->assignment;
        node->toAdd = 0;
    }
    void pushRev(item* &parent,item* &node){
        if (node==nullptr)return;
        node->rev ^= parent->rev;
    }
    void pushFlip(item* &parent,item* &node){
        if (node==nullptr)return;
        node->flip ^= parent->flip;
    }
    void pushHelper(item* &node){
        if (node==nullptr)return;
        if (node->assignment != -1){
            node->sum = node->assignment*node->sz;
            node->value = node->assignment;
            node->toAdd = 0;
            pushAssign(node,node->left), pushAssign(node,node->right);
            node->assignment = -1;
        }
    }
    void prop(item* &node){
        if (node == nullptr)return;
        pushHelper(node);
        if (node->toAdd != 0){
            node->sum += (node->toAdd*node->sz);
            node->value += (node->toAdd);
            pushHelper(node->left), pushHelper(node->right);
            pushAdd(node,node->left), pushAdd(node,node->right);
            node->toAdd = 0;
        }

        if (node->flip == 1){
            node->value^=1;
            swap(node->mx0,node->mx1);
            swap(node->suf0,node->suf1);
            swap(node->pref0,node->pref1);
            pushFlip(node,node->left), pushFlip(node,node->right);
            node->flip=0;
        }

        if (node->rev == 1){
            swap(node->left,node->right);
            swap(node->suf0,node->pref0);
            swap(node->suf1,node->pref1);
            pushRev(node,node->left), pushRev(node,node->right);
            node->rev=0;
        }
    }
    void propAll(item* &node){
        if (node==nullptr)return;
        prop(node);
        prop(node->left);
        prop(node->right);
    }
    pnn split(item* &node,int idx){
        if (node == nullptr){
            return {nullptr,nullptr};
        }
        propAll(node);
        if (size(node->left)>=idx){
            pnn ret = split(node->left,idx);
            node->left = ret.second;
            recalc(node);
            return {ret.first,node};
        }
        else {
            pnn ret = split(node->right,idx-size(node->left)-1);
            node->right = ret.first;
            recalc(node);
            return {node,ret.second};
        }
    }
    pnn splitKey(item* &node,int idx){
        if (node == nullptr){
            return {nullptr,nullptr};
        }
        propAll(node);
        if (idx <= node->key){
            pnn ret = splitKey(node->left,idx);
            node->left = ret.second;
            recalc(node);
            return {ret.first,node};
        }
        else {
            pnn ret = splitKey(node->right,idx);
            node->right = ret.first;
            recalc(node);
            return {node,ret.second};
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
    void segmentOperator(int l,int r,int type,int val=0){
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second,r-l+1);
        if (type == 0) // add
            part2.first->toAdd += val;
        else if (type == 1) // assign
            part2.first->assignment = val;
        else if (type == 2) // reverse
            part2.first->rev ^= 1;
        else if (type == 3)
            part2.first->flip ^= 1;
        root = merge(part1.first,merge(part2.first,part2.second));
    }
    int getTheKthElement(item* &node,int k){
        if (getSum(node->left)<k&&(getSum(node)-getSum(node->right))>=k)return node->key;
        if (getSum(node->left)>=k){
            return getTheKthElement(node->left,k);
        }
        else {
            return getTheKthElement(node->right,k-(getSum(node)-getSum(node->right)));
        }
    }
public:
    int size(){
        return size(root);
    }
    int getSum(int l,int r){
        pnn part1 = split(root,l);
        pnn part2 = split(part1.second,r-l+1);
        int ret = (part2.first==nullptr)? 0 : part2.first->sum;
        root=merge(part1.first,merge(part2.first,part2.second));
        return ret;
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
    void flip(int l,int r){
        segmentOperator(l,r,3);
    }
    void cycle_shift(int l,int r){
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second,r-l+1);
        pnn part3 = split(part2.first,r-l);
        root = merge(part1.first,merge(part3.second,merge(part3.first,part2.second)));
    }
    void append(int value){
        root = merge(root,new item(value));
    }
    void insert(int position,int value){
        pnn part1 = split(root,position);
        part1.first = merge(part1.first,new item(value));
        root=merge(part1.first,part1.second);
    }
    void insertKey(int position,int value){
        pnn part1 = splitKey(root,position+1);
        part1.first=splitKey(part1.first,position).first;
        part1.first=merge(part1.first,new item(value,position));
        root=merge(part1.first,part1.second);
    }
    int getSumKey(int l,int r){
        pnn part1 = splitKey(root,l);
        pnn part2 = splitKey(part1.second,r+1);
        int ret = getSum(part2.first);
        root=merge(part1.first,merge(part2.first,part2.second));
        return ret;
    }
    int getTheKthElement(int k){
        return getTheKthElement(root,k);
    }
    void swap_segments(int l1,int l2,int len){
        pnn part1 = split(root,l2);
        pnn part2 = split(part1.first,l1);
        pnn part3 = split(part2.second,len+1);
        pnn part4 = split(part1.second,len+1);
        merge(part2.first,merge(part4.first,merge(part3.second,merge(part3.first,part4.second))));
    }
    void remove(int l,int r){
        pnn part1 = split(root,l);
        pnn part2 = split(part1.second,r-l+1);
        root = merge(part1.first,part2.second);
    }
    int ans(){
        return max(getMx0(root),getMx1(root));
    }
};


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin>>n>>q;
    cin>>s;
    treap tr;
    for (int i=0;i<n;i++){
        tr.append(s[i]-'0');
    }
    while (q--){
        cin>>x;
        if (x==1){
            cin>>x>>y;
            tr.flip(x-1,y-1);
        }
        else if (x==2){
            cin>>x>>y;
            tr.reverse(x-1,y-1);
        }
        cout<<tr.ans()<<"\n";
    }
    return 0;
}
