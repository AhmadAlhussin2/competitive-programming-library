// solution for https://www.spoj.com/problems/IITWPC4D/

#include <bits/stdc++.h>

#define LL long long
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
    item *left = nullptr, *right = nullptr;
    item(int value, int key = 0) : value(value), key(key)
    {
        priority = getRand(0, INT_MAX);
        sum = value;
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
    void recalc(item *&node)
    {
        node->sz = 1 + size(node->left) + size(node->right);
        node->sum = node->value + getSum(node->left) + getSum(node->right);
    }
    int getSum(item *&node)
    {
        return (node == nullptr) ? 0 : node->sum;
    }
    void pushAdd(item *&parent, item *&node)
    {
        if (node == nullptr)
            return;
        node->toAdd += parent->toAdd;
    }
    void pushAssign(item *&parent, item *&node)
    {
        if (node == nullptr)
            return;
        node->assignment = parent->assignment;
        node->toAdd = 0;
    }
    void pushRev(item *&parent, item *&node)
    {
        if (node == nullptr)
            return;
        node->rev ^= parent->rev;
    }
    void pushHelper(item *&node)
    {
        if (node == nullptr)
            return;
        if (node->assignment != -1)
        {
            node->sum = node->assignment * node->sz;
            node->value = node->assignment;
            node->toAdd = 0;
            pushAssign(node, node->left), pushAssign(node, node->right);
            node->assignment = -1;
        }
    }
    void prop(item *&node)
    {
        if (node == nullptr)
            return;
        pushHelper(node);
        if (node->toAdd != 0)
        {
            node->sum += (node->toAdd * node->sz);
            node->value += (node->toAdd);
            pushHelper(node->left), pushHelper(node->right);
            pushAdd(node, node->left), pushAdd(node, node->right);
            node->toAdd = 0;
        }

        if (node->rev == 1)
        {
            swap(node->left, node->right);
            pushRev(node, node->left), pushRev(node, node->right);
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
        root = merge(part1.first, merge(part2.first, part2.second));
    }
    int getTheKthElement(item *&node, int k)
    {
        propAll(node);
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
    void cycle_shift(int l, int r)
    {
        pnn part1 = split(root, l);
        pnn part2 = split(part1.second, r - l + 1);
        pnn part3 = split(part2.first, r - l);
        root = merge(part1.first, merge(part3.second, merge(part3.first, part2.second)));
    }
    void append(int value)
    {
        root = merge(root, new item(value, size(root)));
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
        pnn part3 = split(part2.second, len + 1);
        pnn part4 = split(part1.second, len + 1);
        merge(part2.first, merge(part4.first, merge(part3.second, merge(part3.first, part4.second))));
    }
};
int t[N], u[N];

int32_t main()
{
    scanf("%d", &q);
    for (int t = 1; t <= q; t++)
    {
        treap tr;
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
        {
            scanf("%d", &a[i]);
            tr.append(1);
        }
        bool ok = 1;
        vector<int> ans;
        for (int i = n; i >= 1; i--)
        {
            int element = i - a[i];
            if (element < 1 || element > tr.getSum(0, n - 1))
            {
                ok = 0;
                break;
            }
            int tmp = tr.getTheKthElement(element);
            ans.push_back(tmp);
            tr.assign(tmp, tmp, 0);
        }
        reverse(ans.begin(), ans.end());
        printf("Test : %d\n", t);
        if (ok == 0)
        {
            printf("-1\n");
        }
        else
        {
            for (int x : ans)
            {
                printf("%d ", x + 1);
            }
            printf("\n");
        }
    }

    return 0;
}