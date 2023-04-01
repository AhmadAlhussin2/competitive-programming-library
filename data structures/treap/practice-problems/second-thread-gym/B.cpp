// solution for https://codeforces.com/gym/102787/problem/B

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
const int M1=805306457;
int hsh1[N];
int pr1=31;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int getRand(int l,int r){
    uniform_int_distribution < int > uid(l,r);
    return uid(rng);
}

struct item{
    int priority, value, sz=1, toAdd=0, sum, assignment=-1, rev=0, key=0;
    int hsh,rev_hsh;
    item *left = nullptr ,*right = nullptr;
    item(int value,int key=0):value(value),key(key){
        priority = getRand(0,INT_MAX);
        sum=value;
        hsh=value;
        rev_hsh=value;
    }
};


int MULT(LL a,LL b){
    return (a*b)%M1;
}
int ADD(int a,int b){
    return (a+b)%M1;
}

#define pnn pair < item* , item* >

class treap{
private:
    item *root = nullptr;
    int size(item* &node){
        return (node==nullptr) ? 0 : node->sz;
    }
    int getHsh(item* &node){
        return (node==nullptr) ? 0 : node->hsh;
    }
    int getRevHsh(item* &node){
        return (node==nullptr) ? 0 : node->rev_hsh;
    }
    void recalc(item* &node){
        node->sz=1+size(node->left)+size(node->right);

        node->hsh=ADD(getHsh(node->left),MULT(node->value,hsh1[size(node->left)]) );
        node->hsh=ADD(node->hsh,MULT(getHsh(node->right),hsh1[size(node->left)+1]));

        node->rev_hsh=ADD(getRevHsh(node->right),MULT(node->value,hsh1[size(node->right)]));
        node->rev_hsh=ADD(node->rev_hsh,MULT(getRevHsh(node->left),hsh1[size(node->right)+1]));
    }
    int getSum(item* &node){
        return (node==nullptr)?0:node->sum;
    }
    pnn split(item* &node,int idx){
        if (node == nullptr){
            return {nullptr,nullptr};
        }
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
    item* merge(item* L,item* R){
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

public:
    int size(){
        return size(root);
    }
    void append(int value){
        root = merge(root,new item(value));
    }
    void insert(int position,int value){
        pnn part1 = split(root,position);
        part1.first = merge(part1.first,new item(value));
        root=merge(part1.first,part1.second);
    }
    void remove(int l,int r){
        pnn part1 = split(root,l);
        pnn part2 = split(part1.second,r-l+1);
        root = merge(part1.first,part2.second);
    }
    bool isPalindrome(int l,int r){
        pnn part1 = split(root,l);
        pnn part2 = split(part1.second,r-l+1);
        bool ret=(part2.first->hsh == part2.first->rev_hsh);
        merge(part1.first,merge(part2.first,part2.second));
        return ret;
    }
};

char c;
int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    hsh1[0]=1;
    for (int i=1;i<N;i++){
        hsh1[i]=((long long)(hsh1[i-1])*pr1)%M1;
    }
    treap tr;
    cin>>n>>q;
    cin>>s;
    s1=s;
    reverse(all(s1));
    for (int i=0;i<s.size();i++){
        tr.append(s[i]-'a'+1);
    }
    while (q--){
        cin>>x;
        if (x==1){
            cin>>x>>y;
            x--,y--;
            tr.remove(x,y);
        }
        else if (x==2){
            cin>>c>>x;
            x--;
            tr.insert(x,c-'a'+1);
        }
        else {
            cin>>x>>y;
            x--,y--;
            if (tr.isPalindrome(x,y))cout<<"yes\n";
            else cout<<"no\n";
        }
    }
    return 0;
}
