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

bool bfs(const vector<vector<ll>>& g, vector<int>& d, const int start, const int end) {
    for (auto& dist : d) {
        dist = 0;
    }
    d[start] = 1;
    queue<int> q;
    q.push(start);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        for (int to = 1; to < g[cur].size(); ++to) {
            if (!g[cur][to] || d[to]) continue;

            d[to] = d[cur] + 1;
            q.push(to);
        }
    }

    return d[end];
}

ll dfs(vector<vector<ll>>& g, vector<int>& ptr, const vector<int>& d, int cur, ll mnD, const int end) {
    if (cur == end) return mnD;

    for (; ptr[cur] < g[cur].size(); ++ptr[cur]) {
        auto to = ptr[cur];

        if (!g[cur][to] || d[to] != d[cur] + 1) continue;

        ll flow = dfs(g, ptr, d, to, min(mnD, g[cur][to]), end);

        if (flow) {
            g[cur][to] -= flow;
            g[to][cur] += flow;
            return flow;
        }
    }

    return 0;
}

ll dinic(vector<vector<ll>>& g, const int start, const int end) {
    const int n = g.size() - 1;

    ll F = 0;
    vector<int> d(n + 1);
    vector<int> ptr(n + 1);
    while (true) {
        if (!bfs(g, d, start, end)) break;

        for (auto &i: ptr) {
            i = 1;
        }

        while (ll delta = dfs(g, ptr, d, start, INF, end)) {
            F += delta;
        }
    }

    return F;
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<ll>> g(n + 1, vector<ll> (n + 1));
    for (int i = 0; i < m; ++i) {
        int from, to, c;
        cin >> from >> to >> c;
        g[from][to] = c;
        g[to][from] = c; // if graph is undirected
    }

    cout << dinic(g, 1, n) << endl;
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
