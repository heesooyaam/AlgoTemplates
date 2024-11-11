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

struct TState {
    int v;
    ll delta;
};

template<class T>
void clear(queue<T>& q) {
    while (!q.empty()) {
        q.pop();
    }
}

ll bfs(vector<unordered_map<int, ll>>& g, const int start, const int end, const ull border) {
    queue<TState> q;
    vector<int> used(g.size());
    vector<int> prev(g.size());
    int color = 1;

    ll F = 0, delta;

    auto add_children = [&](const int cur, const ll d) {
        for (const auto& [to, c] : g[cur]) {
            if (used[to] == color || c < border) continue;

            prev[to] = cur;

            if (to == end) {
                delta = min(d, c);
                return true;
            }

            q.emplace(to, min(d, c));
        }

        return false;
    };

    do {
        delta = 0;
        clear(q);
        q.emplace(start, INF);

        while (!q.empty()) {
            auto [cur, d] = q.front();

            used[cur] = color;
            q.pop();

            if (add_children(cur, d)) {
                break;
            }
        }

        if (delta) {
            ++color;
            F += delta;
            int v = end;
            while (prev[v]) {
                g[prev[v]][v] -= delta;
                g[v][prev[v]] += delta;
                v = prev[v];
            }
        }

    } while (delta);

    return F;
}

ll scalling(vector<unordered_map<int, ll>>& g, const int start, const int end, const ull maxFlow) {
    ull border = max(bit_floor(maxFlow), 1ull);
    ll F = 0;
    while (border) {
        F += bfs(g, start, end, border);
        border >>= 1;
    }

    return F;
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<unordered_map<int, ll>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int from, to, c;
        cin >> from >> to >> c;
        g[from][to] = c;
        g[to][from] = c; // if graph is undirected
    }

    cout << scalling(g, 1, n, 1ull * n * n * (ull) 1e9 + 20) << endl;
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
