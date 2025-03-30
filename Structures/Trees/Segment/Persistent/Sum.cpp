#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
#define eb emplace_back
#define pb push_back
#define pf push_front
#define ppb pop_back
#define ppf pop_front
#define ss second
#define ff first
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define inf numeric_limits<int>::max()
#define INF numeric_limits<ll>::max()
#define print(x); for(auto& val : x){cout << val << ' ';} cout << endl;
#define print1(x); for (int i_ = 1; i_ < (x).size(); ++i_) {cout << (x)[i_] << ' ';} cout << endl;
#define input(x); for(auto& val : x){cin >> val;}
#define input1(x); for (int i_ = 1; i_ < (x).size(); ++i_) {cin << (x)[i_] << ' ';}
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())
#define endl '\n'

template<typename T = int>
class SumTree {
public:
    struct Node {
        T sum = 0;
        int l = 0, r = 0;
    };

    SumTree(const int sz) : n(sz) {
        nodes.eb();
    }

    int update(int pos, T val, int version) {
        return update(version, pos, val, 0, n);
    }

    int get_sum(int l, int r, int version) {
        return get_sum(version, l, r, 0, n);
    }

protected:
    int n;
    vector<Node> nodes;
private:
    int update(int v, int pos, T val, int curL, int curR) {
        if (curL + 1 == curR) {
            nodes.eb(val);
            return nodes.size() - 1;
        }

        int mid = (curL + curR) >> 1;

        nodes.eb(nodes[v]);
        int new_v = nodes.size() - 1;

        int l_child = nodes[new_v].l, r_child = nodes[new_v].r;

        if (pos < mid) {
            nodes[new_v].l = l_child = update(nodes[v].l, pos, val, curL, mid);
        } else {
            nodes[new_v].r = r_child = update(nodes[v].r, pos, val, mid, curR);
        }

        nodes[new_v].sum = nodes[l_child].sum + nodes[r_child].sum;

        return new_v;
    }

    T get_sum(int v, int l, int r, int curL, int curR) {
        if (l >= r) {
            return 0;
        }

        if (l == curL && r == curR) {
            return nodes[v].sum;
        }

        int mid = (curR + curL) >> 1;

        return get_sum(nodes[v].l, l, min(r, mid), curL, mid) + get_sum(nodes[v].r, max(l, mid), r, mid, curR);
    }
};

class KthOneTree : public SumTree<int> {
public:
    KthOneTree(const int sz) : SumTree<int>(sz) {}

    int get_kth_one(int k, int version) {
        return nodes[version].sum < k ? n : get_kth_one(version, k, 0, n);
    }
private:

    int get_kth_one(int v, int k, int curL, int curR) {
        if (curL + 1 == curR) {
            return curL;
        }

        int mid = (curL + curR) >> 1;

        if (nodes[nodes[v].l].sum < k) {
            k -= nodes[nodes[v].l].sum;
            return get_kth_one(nodes[v].r, k, mid, curR);
        } else {
            return get_kth_one(nodes[v].l, k, curL, mid);
        }
    }
};

// sum tested on:      https://codeforces.com/problemset/problem/813/E
// k-th one tested on: https://codeforces.com/contest/787/problem/E

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    input(a);

    vector<int> next(n + 1, -1);
    vector<int> versions;
    versions.reserve(n);

    KthOneTree ST(n);

    int last_v = 0;
    for (int i = n - 1; i >= 0; --i) {
        if (next[a[i]] != -1) {
            last_v = ST.update(next[a[i]], 0, last_v);
        }

        last_v = ST.update(i, 1, last_v);
        versions.eb(last_v);

        next[a[i]] = i;
    }

    reverse(all(versions));

    for (int k = 1; k <= n; ++k) {
        int i = 0;
        int cnt = 0;
        while (i < n) {
            i = ST.get_kth_one(k + 1, versions[i]);
            ++cnt;
        }

        cout << cnt << ' ';
    }
    cout << endl;
}

int32_t main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ios::sync_with_stdio(0); cin.tie(0);
    int ttest = 1;
//    cin >> ttest;
    while(ttest--) solve();
    return 0;
}
