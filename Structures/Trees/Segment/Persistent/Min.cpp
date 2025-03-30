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
#define input1(x); for (int i_ = 1; i_ < (x).size(); ++i_) { cin >> (x)[i_]; }
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())
#define endl '\n'

template<typename T = int>
class PersMinTree {
public:
    struct Node {
        T mn = numeric_limits<T>::max();
        int l = 0, r = 0;
    };

    explicit PersMinTree(int sz) : n(sz) {
        versions.eb();
    }

    int update(int pos, T val, int version) {
        return update(version, pos, val, 0, n);
    }

    T get(int l, int r, int version) {
        return get(version, l, r, 0, n);
    }
private:
    const int n;
    vector<Node> versions;

    T get(int v, int l, int r, int curL, int curR) {
        if (l >= r) {
            return numeric_limits<T>::max();
        }

        if (curL == l && curR == r) {
            return versions[v].mn;
        }

        int mid = (curL + curR) >> 1;

        T mn_l = versions[v].l ? get(versions[v].l, l, min(mid, r), curL, mid) : numeric_limits<T>::max();
        T mn_r = versions[v].r ? get(versions[v].r, max(l, mid), r, mid, curR) : numeric_limits<T>::max();

        return min(mn_l, mn_r);
    }

    int update(int v, int pos, T val, int curL, int curR) {
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

// tested on: https://codeforces.com/contest/893/problem/F

void solve() {
    int n, r;
    cin >> n >> r;
    vector<int> a(n + 1);
    input1(a);

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].eb(v);
        g[v].eb(u);
    }

    int time = 0;
    vector<int> tin(n + 1), tout(n + 1), euler;
    euler.reserve(n);

    auto dfs = [&g, &tin, &tout, &time, &euler](auto&& dfs, int cur, int prev = -1) -> void {
        euler.eb(cur);
        tin[cur] = time;

        for (auto& nxt : g[cur]) {
            if (nxt == prev) continue;

            ++time;
            dfs(dfs, nxt, cur);
        }

        tout[cur] = time;
    };
    dfs(dfs, r);

    PersMinTree ST(n);
    assert(euler.size() == n);
    vector<int> versions;
    versions.reserve(n);

    vector<int> h(n + 1, -1);

    queue<int> q;
    q.push(r);
    h[r] = 0;
    int prev = 0;
    int last_v = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        if (h[cur] == prev + 1) {
            versions.eb(last_v);
        }

        last_v = ST.update(tin[cur], a[cur], last_v);
        prev = h[cur];

        for (const auto to : g[cur]) {
            if (h[to] != -1) continue;

            h[to] = h[cur] + 1;
            q.push(to);
        }
    }
    versions.eb(last_v);

    int m;
    cin >> m;
    int last = 0;
    while (m--) {
        int p, q_;
        cin >> p >> q_;
        int x = (p + last) % n + 1, k = (q_ + last) % n;
        int v = versions[min(h[x] + k, (int) versions.size() - 1)];
        last = ST.get(tin[x], tout[x] + 1, v);
        cout << last << endl;
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