// solution for https://codeforces.com/gym/102787/problem/D

#include <bits/stdc++.h>

#define int long long
#define pb push_back
#define F first
#define S second
#define all(x) x.begin(), x.end()

const double eps = 1e-7, PI = 3.14159265358979323846;
const int N = 1e6 + 10;

using namespace std;

int n, q, m, k, x, y, a[N], mx = -1, mn = 1e9, sum;
string s, s1, s2;
map<int, int> mp;
vector<int> vec;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int getRand(int l, int r)
{
    uniform_int_distribution<int> uid(l, r);
    return uid(rng);
}

struct item
{
    int priority, value, sz = 1, toAdd = 0, sum, assignment = -1, rev = 0, key = 0;
    int mx,secondMx,cntMx;
    item *left = nullptr, *right = nullptr;
    item(int value, int key = 0) : value(value), key(key), sum(value)
    {
        priority = getRand(0, INT_MAX);
        mx=value;
        secondMx=INT_MIN;
        cntMx=1;
    }
};

#define pnn pair<item *, item *>

class treap
{
private:
    item *root = nullptr;
    int size(item *&node)
    {
        return (node == nullptr) ? 0 : node->sz;
    }
    int getMax(item *&node){
        return (node == nullptr) ? INT_MIN : node->mx;
    }
    int getSecondMax(item *&node){
        return (node == nullptr) ? INT_MIN : node->secondMx;
    }
    void recalc(item *&node)
    {
        node->sz = 1 + size(node->left) + size(node->right);
        node->sum = node->value + getSum(node->left) + getSum(node->right);

        node->mx = max({node->value, getMax(node->left), getMax(node->right)});
        node->secondMx = max(getSecondMax(node->left), getSecondMax(node->right));

        node->cntMx = 0;
        if (node->mx == node->value)
            node->cntMx += 1;
        else
            node->secondMx = max(node->secondMx, node->value);

        if (getMax(node->left) == node->mx)
            node->cntMx += node->left->cntMx;
        else
            node->secondMx = max(node->secondMx, getMax(node->left));

        if (getMax(node->right) == node->mx)
            node->cntMx += node->right->cntMx;
        else
            node->secondMx = max(node->secondMx, getMax(node->right));
    }
    int getSum(item *&node)
    {
        return (node == nullptr) ? 0 : node->sum;
    }
    void push(item *&parent, item *&node,int type)
    {
        if (node == nullptr)
            return;
        if (type == 1)
            node->toAdd += parent->toAdd;
        if (type == 2){
            node->assignment = parent->assignment,node->toAdd = 0;
            node->mx = node->assignment;
            node->secondMx = INT_MIN;
            node->cntMx = node->sz;
        }
        if (type == 3)
            node->rev ^= parent->rev;
    }
    void assign(item *&node)
    {
        if (node == nullptr)
            return;
        if (node->assignment != -1)
        {
            node->sum = node->assignment * node->sz;
            node->value = node->assignment;
            node->toAdd = 0;
            node->mx = node->assignment;
            node->secondMx = INT_MIN;
            node->cntMx = node->sz;
            push(node, node->left,2), push(node, node->right,2);
            node->assignment = -1;
        }
    }
    void pushSum(item *& node){
        if (node==nullptr)return;
        if (node->toAdd != 0)
        {
            node->sum += (node->toAdd * node->sz);
            node->value += (node->toAdd);
            node->mx += node->toAdd;
            if (node->secondMx >=0)node->secondMx += node->toAdd;
            assign(node->left), assign(node->right);
            push(node, node->left,1), push(node, node->right,1);
            node->toAdd = 0;
        }
    }
    void setMin(int value,item *&node){
        if (node == nullptr)
            return;
        assign(node);
        pushSum(node);
        if (node->secondMx >= value){
            node->sum -= max((int)0,node->value-value);
            node->value = min(node->value,value);
            setMin(value,node->left);
            setMin(value,node->right);
            recalc(node);
        }
        else if (node->mx > value){
            node->sum -= ((node->mx-value) * node->cntMx);
            node->value = min(node->value,value);
            node->mx=value;
        }
    }
    void prop(item *&node)
    {
        if (node == nullptr)
            return;
        assign(node);
        pushSum(node);
        setMin(node->mx,node);
        setMin(node->mx,node->left);
        setMin(node->mx,node->right);
        if (node->rev == 1)
        {
            swap(node->left, node->right);
            push(node, node->left,3), push(node, node->right,3);
            node->rev = 0;
        }
    }
    void propAll(item *&node)
    {
        if (node == nullptr)
            return;
        prop(node);
        prop(node->left);
        prop(node->right);
    }
    pnn split(item *&node, int idx)
    {
        if (node == nullptr)
        {
            return {nullptr, nullptr};
        }
        propAll(node);
        if (size(node->left) >= idx)
        {
            pnn ret = split(node->left, idx);
            node->left = ret.second;
            recalc(node);
            return {ret.first, node};
        }
        else
        {
            pnn ret = split(node->right, idx - size(node->left) - 1);
            node->right = ret.first;
            recalc(node);
            return {node, ret.second};
        }
    }
    pnn splitKey(item *&node, int idx)
    {
        if (node == nullptr)
        {
            return {nullptr, nullptr};
        }
        propAll(node);
        if (idx <= node->key)
        {
            pnn ret = splitKey(node->left, idx);
            node->left = ret.second;
            recalc(node);
            return {ret.first, node};
        }
        else
        {
            pnn ret = splitKey(node->right, idx);
            node->right = ret.first;
            recalc(node);
            return {node, ret.second};
        }
    }
    item *merge(item *L, item *R)
    {
        propAll(L);
        propAll(R);
        if (L == nullptr)
            return R;
        if (R == nullptr)
            return L;
        if (L->priority > R->priority)
        {
            L->right = merge(L->right, R);
            recalc(L);
            return L;
        }
        else
        {
            R->left = merge(L, R->left);
            recalc(R);
            return R;
        }
    }
    void segmentOperator(int l, int r, int type, int val = 0)
    {
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second, r - l + 1);
        if (type == 0) // add
            part2.first->toAdd += val;
        else if (type == 1) // assign
            part2.first->assignment = val;
        else if (type == 2) // reverse
            part2.first->rev ^= 1;
        else if (type ==3)
            setMin(val,part2.first);
        root = merge(part1.first, merge(part2.first, part2.second));
    }
    int getTheKthElement(item *&node, int k)
    {
        if (getSum(node->left) < k && (getSum(node) - getSum(node->right)) >= k)
            return node->key;
        if (getSum(node->left) >= k)
        {
            return getTheKthElement(node->left, k);
        }
        else
        {
            return getTheKthElement(node->right, k - (getSum(node) - getSum(node->right)));
        }
    }

public:
    int size()
    {
        return size(root);
    }
    int getSum(int l, int r)
    {
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second, r - l + 1);
        int ret = (part2.first == nullptr) ? 0 : part2.first->sum;
        root = merge(part1.first, merge(part2.first, part2.second));
        return ret;
    }
    void add(int l, int r, int val)
    {
        segmentOperator(l, r, 0, val);
    }
    void assign(int l, int r, int val)
    {
        segmentOperator(l, r, 1, val);
    }
    void reverse(int l, int r)
    {
        segmentOperator(l, r, 2);
    }
    void setMax(int l,int r,int val){
        segmentOperator(l, r, 3,val);
    }
    void cycle_shift(int l, int r)
    {
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second, r - l + 1);
        pnn part3 = split(part2.first, r - l);
        root = merge(part1.first, merge(part3.second, merge(part3.first, part2.second)));
    }
    void append(int value)
    {
        root = merge(root, new item(value));
    }
    void insert(int position, int value)
    {
        pnn part1 = split(root, position);
        part1.first = merge(part1.first, new item(value));
        root = merge(part1.first, part1.second);
    }
    void insertKey(int position, int value)
    {
        pnn part1 = splitKey(root, position + 1);
        part1.first = splitKey(part1.first, position).first;
        part1.first = merge(part1.first, new item(value, position));
        root = merge(part1.first, part1.second);
    }
    int getSumKey(int l, int r)
    {
        pnn part1 = splitKey(root, l);
        pnn part2 = splitKey(part1.second, r + 1);
        int ret = getSum(part2.first);
        root = merge(part1.first, merge(part2.first, part2.second));
        return ret;
    }
    int getTheKthElement(int k)
    {
        return getTheKthElement(root, k);
    }
    void swap_segments(int l1, int l2, int len)
    {
        pnn part1 = split(root, l2);
        pnn part2 = split(part1.first, l1);
        pnn part3 = split(part2.second, len);
        pnn part4 = split(part1.second, len);
        merge(part2.first, merge(part4.first, merge(part3.second, merge(part3.first, part4.second))));
    }
    void remove(int l, int r)
    {
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second, r - l + 1);
        root = merge(part1.first, part2.second);
    }
    void move(int l,int r){
        pnn part1 = split(root,l);
        pnn part2 = split(part1.second,r-l+1);
        root = merge(part1.first,merge(part2.second,part2.first));
    }
};

int32_t main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin>>n>>q;
    treap tr;
    for (int i=0;i<n;i++){
        cin>>x;
        tr.append(x);
    }
    while (q--){
        cin>>x;
        if (x==1){
            cin>>x>>y>>k;
            tr.setMax(x-1,y-1,k);
        }
        else if (x==2){
            cin>>x>>y;
            tr.move(x-1,y-1);
        }
        else {
            cin>>x>>y>>k;
            tr.add(x-1,y-1,k);
        }
        cout<<tr.getSum(0,n-1)<<"\n";
    }
    return 0;
}
