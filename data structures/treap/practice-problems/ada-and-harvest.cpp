// solution for https://www.spoj.com/problems/ADACROP/

#include <bits/stdc++.h>

#define LL long long
#define pb push_back
#define F first
#define S second
#define all(x) x.begin(), x.end()

const double eps = 1e-7, PI = 3.1415926;
const int N = 1e6 + 10;

using namespace std;

int n, q, m, k, x, y, a[N], mx = -1, mn = 1e9, sum;
string s, s1, s2;
map<int, int> mp;
vector<pair<int, int>> vec;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
class treapset
{
private:
    struct item;
    typedef struct item *pitem;
    bool _is_sorted = true, stop;
    pitem root = NULL, _begin = NULL;
    int next_priority()
    {
        int prior = rnd();
        return prior;
    }
    void regenerate_priorities_recursive(vector<int> &new_priors, pitem &t, int l, int r)
    {
        if (!t)
            return;
        t->prior = new_priors[r - 1];
        regenerate_priorities_recursive(new_priors, t->l, l, l + cnt(t->l));
        regenerate_priorities_recursive(new_priors, t->r, l + cnt(t->l), r - 1);
    }
    void regenerate_priorities()
    {
        int sz = size();
        vector<int> new_priors(sz);
        for (int i = 0; i < sz; i++)
            new_priors[i] = next_priority();
        sort(new_priors.begin(), new_priors.end());
        for (int i = 0; i < sz; i++)
            new_priors[i] += i;
        regenerate_priorities_recursive(new_priors, root, 0, sz);
    }
    struct item
    {
        int prior, cnt, rev, key, add, fsum;
        pitem l, r;
        item(int x, int p)
        {
            add = 0;
            key = fsum = x;
            cnt = 1;
            rev = 0;
            l = r = NULL;
            prior = p;
        }
    };
    int cnt(pitem it)
    {
        return it ? it->cnt : 0;
    }
    void upd_cnt(pitem it)
    {
        if (it)
            it->cnt = cnt(it->l) + cnt(it->r) + 1;
    }
    void upd_sum(pitem it)
    {
        if (it)
        {
            it->fsum = it->key;
            if (it->l)
                it->fsum += it->l->fsum;
            if (it->r)
                it->fsum += it->r->fsum;
        }
    }
    void update(pitem t, int add, int rev)
    {
        if (!t)
            return;
        t->add = t->add + add;
        t->rev = t->rev ^ rev;
        t->key = t->key + add;
        t->fsum = t->fsum + cnt(t) * add;
    }
    void push(pitem t)
    {
        if (!t || (t->add == 0 && t->rev == 0))
            return;
        update(t->l, t->add, t->rev);
        update(t->r, t->add, t->rev);
        if (t->rev)
            swap(t->l, t->r);
        t->add = 0;
        t->rev = 0;
    }
    void merge(pitem &t, pitem l, pitem r)
    {
        push(l);
        push(r);
        if (!l || !r)
            t = l ? l : r;
        else if (l->prior > r->prior)
            merge(l->r, l->r, r), t = l;
        else
            merge(r->l, l, r->l), t = r;
        upd_cnt(t);
        upd_sum(t);
    }
    void split(pitem t, pitem &l, pitem &r, int index)
    { // split at position
        if (!t)
            return void(l = r = 0);
        push(t);
        if (index <= cnt(t->l))
            split(t->l, l, t->l, index), r = t;
        else
            split(t->r, t->r, r, index - 1 - cnt(t->l)), l = t;
        upd_cnt(t);
        upd_sum(t);
    }
    void splitK(pitem t, pitem &l, pitem &r, int &key, bool &eq)
    { // split by key
        if (!t)
            return void(l = r = 0);
        push(t);
        if (key == t->key)
        {
            eq = true;
            return;
        }
        if (key < t->key)
        {
            splitK(t->l, l, t->l, key, eq);
            if (!eq)
                r = t;
        }
        else
        {
            splitK(t->r, t->r, r, key, eq);
            if (!eq)
                l = t;
        }
        if (!eq)
            upd_cnt(t);
        upd_sum(t);
    }

    void insert(pitem &t, pitem it, int index)
    { // insert at position
        push(t);
        if (!t)
            t = it;
        else if (it->prior == t->prior)
        {
            stop = true;
            regenerate_priorities();
        }
        else if (it->prior > t->prior)
            split(t, it->l, it->r, index), t = it;
        else if (index <= cnt(t->l))
            insert(t->l, it, index);
        else
            insert(t->r, it, index - cnt(t->l) - 1);
        if (stop)
            return;
        upd_cnt(t);
        upd_sum(t);
    }

    void insertK(pitem &t, pitem it, bool &eq)
    { // insert by key
        push(t);
        if (!t)
            t = it;
        else if (it->key == t->key)
        {
            eq = true;
            return;
        }
        else if (it->prior == t->prior)
        {
            stop = true;
            regenerate_priorities();
        }
        else if (it->prior > t->prior)
        {
            splitK(t, it->l, it->r, it->key, eq);
            if (!eq)
                t = it;
        }
        else if (it->key < t->key)
            insertK(t->l, it, eq);
        else
            insertK(t->r, it, eq);
        if (stop)
            return;
        if (!eq)
            upd_cnt(t);
        upd_sum(t);
    }

    void erase(pitem &t, int index)
    {
        push(t);
        if (cnt(t->l) == index)
            merge(t, t->l, t->r);
        else if (index < cnt(t->l))
            erase(t->l, index);
        else
            erase(t->r, index - cnt(t->l) - 1);
        upd_cnt(t);
        upd_sum(t);
    }

    void eraseK(pitem &t, int key, bool &found)
    {
        push(t);
        if (key == t->key)
        {
            merge(t, t->l, t->r);
            found = true;
        }
        else if (key < t->key)
            eraseK(t->l, key, found);
        else
            eraseK(t->r, key, found);
        upd_cnt(t);
        upd_sum(t);
    }

    int get(pitem t, int index)
    {
        push(t);
        if (index < cnt(t->l))
            return get(t->l, index);
        else if (index > cnt(t->l))
            return get(t->r, index - cnt(t->l) - 1);
        return t->key;
    }

    int find(pitem t, int key)
    {
        push(t);
        if (!t || key == t->key)
            return cnt(t->l);
        if (key < t->key)
            return get(t->l, key);
        else
            return get(t->r, key) + 1 + cnt(t->l);
    }

    pair<int, int> lower_bound(pitem t, int key, int index)
    {
        push(t);
        if (!t)
            return make_pair(0, size());
        if (key == t->key)
            return make_pair(key, index + cnt(t->l));
        if (key < t->key)
        {
            pair<int, int> ret = lower_bound(t->l, key, index);
            if (ret.second == size())
                ret = make_pair(t->key, index + cnt(t->l));
            return ret;
        }
        return lower_bound(t->r, key, index + 1 + cnt(t->l));
    }

    pair<int, int> upper_bound(pitem t, int key, int index)
    {
        push(t);
        if (!t)
            return make_pair(0, size());
        if (key < t->key)
        {
            pair<int, int> ret = upper_bound(t->l, key, index);
            if (ret.second == size())
                ret = make_pair(t->key, index + cnt(t->l));
            return ret;
        }
        return upper_bound(t->r, key, index + 1 + cnt(t->l));
    }

    void shift(pitem &t, int l, int r, int add)
    {
        pitem l1, r1;
        split(t, l1, r1, r + 1);
        pitem l2, r2;
        split(l1, l2, r2, l);
        update(r2, add, 0);
        pitem t2;
        merge(t2, l2, r2);
        merge(t, t2, r1);
    }

    void reverse(pitem &t, int l, int r)
    {
        pitem l1, r1;
        split(t, l1, r1, r + 1);
        pitem l2, r2;
        split(l1, l2, r2, l);
        update(r2, 0, 1);
        pitem t2;
        merge(t2, l2, r2);
        merge(t, t2, r1);
    }

    int query_sum(pitem &t, int l, int r)
    {
        pitem l1, r1;
        split(t, l1, r1, r + 1);
        pitem l2, r2;
        split(l1, l2, r2, l);
        int ret = r2->fsum;
        pitem t2;
        merge(t2, l2, r2);
        merge(t, t2, r1);
        return ret;
    }

public:
    int size() { return cnt(root); }
    bool empty() { return (cnt(root) == 0); }
    bool is_sorted() { return _is_sorted; }
    bool insert(int x)
    {
        if (!_is_sorted)
            return false;
        bool eq = false;
        pitem it = new item(x, next_priority());
        stop = false;
        insertK(root, it, eq);
        while (stop)
        {
            stop = false;
            eq = false;
            insertK(root, it, eq);
        }
        return !eq;
    }
    void insert_pos(int pos, int x)
    {
        if (pos > size())
            return;
        pitem it = new item(x, next_priority());
        stop = false;
        insert(root, it, pos);
        while (stop)
        {
            stop = false;
            insert(root, it, pos);
        }
        if (pos > 0 && _is_sorted)
        {
            if (get(root, pos - 1) >= get(root, pos))
                _is_sorted = false;
        }
        if (pos < size() - 1 && _is_sorted)
        {
            if (get(root, pos) >= get(root, pos + 1))
                _is_sorted = false;
        }
    }
    bool erase(int x)
    {
        if (!_is_sorted)
            return false;
        bool found = false;
        eraseK(root, x, found);
        return found;
    }
    void erase_pos(int pos)
    {
        if (pos >= size())
            return;
        erase(root, pos);
    }

    int get_index(int key)
    {
        if (!_is_sorted)
            return size();
        pitem t = root;
        int index = 0;
        while (t && t->key != key)
        {
            if (t->key > key)
                t = t->l;
            else
            {
                index += cnt(t->l) + 1;
                t = t->r;
            }
        }
        if (!t)
            return size();
        index += cnt(t->l);
        return index;
    }
    int operator[](int index)
    {
        return get(root, index);
    }
    pair<int, int> lower_bound(int x)
    {
        if (!_is_sorted)
            return make_pair(0, size());
        return lower_bound(root, x, 0);
    }
    pair<int, int> upper_bound(int x)
    {
        if (!_is_sorted)
            return make_pair(0, size());
        return upper_bound(root, x, 0);
    }

    void shift(int left, int right, int x)
    {
        left = max(left, 0);
        right = min(right, size() - 1);
        shift(root, left, right, x);
        if (left > 0 && _is_sorted)
        {
            if (get(root, left - 1) >= get(root, left))
                _is_sorted = false;
        }
        if (right < size() - 1 && _is_sorted)
        {
            if (get(root, right) >= get(root, right + 1))
                _is_sorted = false;
        }
    }

    void reverse(int left, int right)
    {
        left = max(left, 0);
        right = min(right, size() - 1);
        reverse(root, left, right);
        if (left != right)
            _is_sorted = false;
    }

    int sum(int left, int right)
    {
        return query_sum(root, left, right);
    }
} t[N];

int main()
{
    set<int> st;
    scanf("%d %d", &n, &q);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
        st.insert(a[i]);
    }
    for (int i = 0; i < q; i++)
    {
        scanf("%d %d", &x, &y);
        vec.pb({x, y});
        st.insert(y);
    }
    int cnt = 0;
    for (auto x : st)
    {
        mp[x] = cnt++;
    }
    for (int i = 0; i < n; i++)
        a[i] = mp[a[i]];
    for (int i = 0; i < vec.size(); i++)
    {
        vec[i].S = mp[vec[i].S];
    }
    for (int i = 0; i < n; i++)
    {
        t[a[i]].insert(i);
    }
    for (auto x : vec)
    {
        if (a[x.F] != x.S)
        {
            t[a[x.F]].erase(x.F);
            a[x.F] = x.S;
            t[x.S].insert(x.F);
        }
        int ans = t[x.S].get_index(x.F);
        printf("%d\n", ans);
    }
    return 0;
}