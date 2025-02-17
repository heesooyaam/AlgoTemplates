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
#define print(x); for(auto& val : x){cout << val << ' ';}cout << endl;
#define input(x); for(auto& val : x){cin >> val;}
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())
#define endl '\n'

class MinTree {
public:
    struct Node {
        int mn = inf;
        int l = 0, r = 0;
    };

    explicit MinTree(int sz) : n(sz) {
        versions.eb();
    }

    int update(int pos, int val, int version) {
        return update(version, pos, val, 0, n);
    }

    int get(int l, int r, int version) {
        return get(version, l, r, 0, n);
    }
private:
    const int n;
    vector<Node> versions;

    int get(int v, int l, int r, int curL, int curR) {
        if (l >= r) {
            return inf;
        }

        if (curL == l && curR == r) {
            return versions[v].mn;
        }

        int mid = (curL + curR) >> 1;

        int mn_l = versions[v].l ? get(versions[v].l, l, min(mid, r), curL, mid) : inf;
        int mn_r = versions[v].r ? get(versions[v].r, max(l, mid), r, mid, curR) : inf;

        return min(mn_l, mn_r);
    }

    int update(int v, int pos, int val, int curL, int curR) {
        if (!v) { // от неявного ДО
            versions.eb();
            v = versions.size() - 1;
        }

        if (curL + 1 == curR) {
            versions.eb(val, 0, 0);
            return versions.size() - 1;
        }

        int mid = (curL + curR) >> 1;

        versions.eb(versions[v]);
        const int new_v = versions.size() - 1;

        int l_child = versions[new_v].l, r_child = versions[new_v].r;
        if (pos < mid) {
            versions[new_v].l = l_child = update(versions[v].l, pos, val, curL, mid);
        } else {
            versions[new_v].r = r_child = update(versions[v].r, pos, val, mid, curR);
        }

        versions[new_v].mn = min(versions[l_child].mn, versions[r_child].mn);

        return new_v;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    MinTree ST(n);
    int last = 0;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        last = ST.update(i, x, last);
    }

    for (int i = 0; i < m; ++i) {
        int q;
        cin >> q;
        if (q == 1) {
            int pos, x;
            cin >> pos >> x;
            last = ST.update(pos, x, last);
        } else {
            int l, r;
            cin >> l >> r;
            cout << ST.get(l, r, last) << endl;
        }
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
