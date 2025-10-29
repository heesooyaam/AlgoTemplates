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
#define input1(x); for (int i_ = 1; i_ < (x).size(); ++i_) {cin >> (x)[i_];}
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())
#define endl '\n'

void dfs(const vector<vector<int>>& g, const int n, vector<array<int, 2>>& dp, int cur, int prev = -1) {
    static const auto is_real = [n](int v) {
        return v <= n;
    };

    auto& curdp = dp[cur];
    curdp[1] = is_real(cur);
    for (int to : g[cur]) {
        if (to == prev) {
            continue;
        }

        dfs(g, n, dp, to, cur);

        const auto& todp = dp[to];
        if (is_real(to)) {
            curdp[0] += max(todp[0], todp[1]);
            curdp[1] += todp[0];
        } else {
            curdp[0] += todp[0];
            curdp[1] += todp[1];
        }
    }
}

struct xevent {
    int id;
    int x;
    bool tp;
    bool operator<(const xevent& other) const {
        if (x != other.x) return x < other.x;
        if (tp != other.tp) return tp < other.tp;
        return id < other.id;
    }
};

struct yevent {
    int id;
    bool tp; // lower - false, upper - true
};

struct circle {
    int x, y, r;
    int parent_id = -1;
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<xevent> xevents;
    vector<circle> find_by_id(n + q + 1);
    for (int i = 1; i < n + q + 1; ++i) {
        int x, y, r;
        cin >> x >> y >> r;
        find_by_id[i] = circle{x, y, r};
        xevents.eb(i, x - r, false);
        xevents.eb(i, x + r, true);
    }

    sort(all(xevents));

    int scanline_x = xevents.begin()->x;
    const auto cmp = [&scanline_x, &find_by_id](const yevent& lhs, const yevent& rhs) {
        if (lhs.id == rhs.id) {
            return lhs.tp < rhs.tp;
        }

        const auto& [xl, yl, rl, _l] = find_by_id[lhs.id];
        const auto& [xr, yr, rr, _r] = find_by_id[rhs.id];

        const auto dyl = max(0ll, 1ll * rl * rl - 1ll * (xl - scanline_x) * (xl - scanline_x));
        const auto dyr = max(0ll, 1ll * rr * rr - 1ll * (xr - scanline_x) * (xr - scanline_x));

        const auto a = yl + sqrtl(dyl) * (lhs.tp ? 1 : -1);
        const auto b = yr + sqrtl(dyr) * (rhs.tp ? 1 : -1);

        return a != b ? a < b : lhs.id < rhs.id;
    };

    set<yevent, decltype(cmp)> yevents(cmp);
    vector<vector<int>> g(n + q + 1);
    for (const auto& [id, x, end] : xevents) {
        scanline_x = x;

        auto& [_, y, r, pid] = find_by_id[id];
        auto it = yevents.lower_bound(yevent{id, false});

        if (!end) {
            if (it != yevents.end()) {
                const int parent = !it->tp ? find_by_id[it->id].parent_id : it->id;
                pid = parent;

                if (pid != -1) {
                    g[parent].eb(id);
                    g[id].eb(parent);
                }
            }

            yevents.emplace(id, false);
            yevents.emplace(id, true);
        } else {
            yevents.erase(yevent{id, false});
            yevents.erase(yevent{id, true});
        }
    }

    vector<array<int, 2>> dp(n + q + 1, {0, 0});
    for (int v = 1; v < n + q + 1; ++v) {
        if (find_by_id[v].parent_id == -1) {
            dfs(g, n, dp, v);
        }
    }

    for (int v = n + 1; v < n + q + 1; ++v) {
        cout << max(dp[v][0], dp[v][1] + 1) << endl;
    }
}

// tested on C from https://techarena-hse25.mrc.msk.ru/campday2 (https://qoj.ac/contest/474)

int32_t main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ios::sync_with_stdio(0); cin.tie(0);
    int ttest = 1;
//    cin >> ttest;
    while(ttest--) solve();
    return 0;
}
