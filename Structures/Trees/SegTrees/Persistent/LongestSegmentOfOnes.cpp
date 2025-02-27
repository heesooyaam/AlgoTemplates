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

class SegTree {
public:
    struct Node {
        int pref = 0, suff = 0, mx = 0;
        bool full = false;
        int l = 0, r = 0;
    };

    SegTree(const int sz) : n(sz) { nodes.eb(); }

    int get(int l, int r, int version) {
        return get(version, l, r, 0, n).mx;
    }

    int update(int pos, int val, int version) {
        return update(version, pos, val, 0, n);
    }

private:
    const int n;
    vector<Node> nodes;

    void sync_changes(const int v) {
        auto &l_child = nodes[nodes[v].l], &r_child = nodes[nodes[v].r];
        nodes[v].pref = l_child.pref + r_child.pref * l_child.full;
        nodes[v].suff = r_child.suff + l_child.suff * r_child.full;
        nodes[v].full = l_child.full && r_child.full;
        nodes[v].mx = max({nodes[v].pref, nodes[v].suff, l_child.mx, r_child.mx, l_child.suff + r_child.pref});
    }

    inline Node merge(const Node& l_node, const Node& r_node) {
        Node merged = {l_node.pref + r_node.pref * l_node.full, r_node.suff + l_node.suff * r_node.full};
        merged.full = l_node.full && r_node.full;
        merged.mx = max({merged.pref, merged.suff, l_node.mx, r_node.mx, l_node.suff + r_node.pref});

        return merged;
    }

    int update(int v, int pos, int val, int curL, int curR) {
        if (curL + 1 == curR) {
            nodes.eb(val, val, val, val);
            return nodes.size() - 1;
        }

        int mid = (curL + curR) >> 1;

        nodes.eb(nodes[v]);
        const int new_v = nodes.size() - 1;

        if (pos < mid) {
            nodes[new_v].l = update(nodes[v].l, pos, val, curL, mid);
        } else {
            nodes[new_v].r = update(nodes[v].r, pos, val, mid, curR);
        }

        sync_changes(new_v);
        return new_v;
    }

    Node get(int v, int l, int r, int curL, int curR) {
        if (l >= r) {
            return Node{};
        }

        if (l == curL && r == curR) {
            return nodes[v];
        }

        int mid = (curL + curR) >> 1;
        return merge(get(nodes[v].l, l, min(r, mid), curL, mid), get(nodes[v].r, max(l, mid), r, mid, curR));
    }
};

// tested on https://codeforces.com/problemset/problem/484/E

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    input(a);
    SegTree ST(n);
    int last_v = 0;
    map<int, vector<int>, greater<>> pos;
    map<int, int> num_to_version;
    for (int i = 0; i < n; ++i) {
        pos[a[i]].eb(i);
    }

    for (auto& [num, vec] : pos) {
        for (auto& i : vec) {
            last_v = ST.update(i, 1, last_v);
        }
        num_to_version[num] = last_v;
    }

    auto check = [&](int h, int l, int r, int need) {
        auto it = num_to_version.lower_bound(h);
        if (it == num_to_version.end()) return false;

        const int need_v = it->ss;
        return ST.get(l, r, need_v) >= need;
    };

    int m;
    cin >> m;
    const int MAXH = *max_element(all(a));
    while (m--) {
        int L, R, w;
        cin >> L >> R >> w;

        int l = 1, r = MAXH;
        int ans = 1;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid, L - 1, R, w)) {
                ans = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        cout << ans << endl;
    }
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
