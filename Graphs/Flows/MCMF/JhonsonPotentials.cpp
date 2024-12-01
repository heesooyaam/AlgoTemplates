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

struct Edge {
    int from, to;
    ll cost;
};

vector<ll> FordBellman(const vector<Edge>& edges, const vector<ll>& capacity, int n, const int start) {
    vector<ll> d(n + 1, INF);
    d[start] = 0;
    bool better = true;

    while (n-- && better) {
        better = false;
        for (int i = 0; i < edges.size(); ++i) {
            if (!capacity[i]) continue;

            const auto& [from, to, w] = edges[i];
            if (d[from] != INF && d[to] > d[from] + w) {
                better = true;
                d[to] = d[from] + w;
            }
        }
    }

    return d;
}

vector<int> GetPath(const vector<int>& prv, const int start, int target) {
    vector<int> path;
    while (target != -1) {
        path.eb(target);
        target = prv[target];
    }
    if (path.back() != start) {
        return {};
    }

    reverse(all(path));
    return path;
}

namespace Matrix {
    // create struct called DijkstraEdge if u wanna store additional info (sample: Graphs/Flows/MCMF/JhonsonPotentials.cpp),
    struct DijkstraEdge {
        ll w, c = 0;
    };

    inline pair<vector<ll>, vector<int>> DijkstraWithPath(const vector<vector<DijkstraEdge>>& g, const vector<ll>& p, const int n, const int start, const int end = -1) {
        vector<bool> used(n + 1);
        vector<int> prv(n + 1, -1);

        vector<ll> d(n + 1, INF);
        d[start] = 0;

        int nearest = start;
        while (true) {
            bool relaxed = false;

            ll min_d = INF;
            for (int v = 0; v < n + 1; ++v) {
                if (used[v] || d[v] == INF || d[v] >= min_d) continue;

                relaxed = true;
                min_d = d[v];
                nearest = v;
            }

            if (!relaxed) break;
            used[nearest] = true;

            for (int to = 0; to < n + 1; ++to) {
                if (used[to] || !g[nearest][to].c) continue;

                if (ll new_d = min_d + g[nearest][to].w + p[nearest] - p[to]; d[to] > new_d) {
                    d[to] = new_d;
                    prv[to] = nearest;
                }
            }
        }

        return mp(d, (end == -1) ? prv : GetPath(prv, start, end));
    }

    inline vector<ll> Dijkstra(const vector<vector<DijkstraEdge>>& g, const vector<ll>& p, const int n, const int start) {
        vector<bool> used(n + 1);

        vector<ll> d(n + 1, INF);
        d[start] = 0;

        int nearest = start;
        while (true) {
            bool relaxed = false;

            ll min_d = INF;
            for (int v = 0; v < n + 1; ++v) {
                if (used[v] || d[v] == INF || d[v] >= min_d) continue;

                relaxed = true;
                min_d = d[v];
                nearest = v;
            }

            if (!relaxed) break;
            used[nearest] = true;

            for (int to = 0; to < g[nearest].size(); ++to) {
                if (used[to] || !g[nearest][to].c) continue;

                d[to] = min(d[to], d[nearest] + g[nearest][to].w + p[nearest] - p[to]);
            }
        }

        return d;
    }
} // namespace Matrix

using namespace Matrix;
using MinCostMaxFlow = pair<ll, ll>;
MinCostMaxFlow MCMF(vector<Edge>& edges, vector<ll>& capacity, const int n, const int start, const int end) {
    const int m = edges.size();

    vector<vector<DijkstraEdge>> g(n + 1, vector<DijkstraEdge>(n + 1));

    auto p{FordBellman(edges, capacity, n, start)};
    for (int i = 0; i < m; ++i) {
        const auto [from, to, cost] = edges[i];

        g[from][to] = {cost, capacity[i]};
    }


    ll F = 0, cost = 0;
    while (true) {
        auto [d, path] = DijkstraWithPath(g, p, n, start, end);

        ll delta = INF;
        for (int i = 0; i + 1 < path.size(); ++i) {
            int from = path[i], to = path[i + 1];
            delta = min(delta, g[from][to].c);
        }

        if (delta == INF) {
            break;
        }

        F += delta;
        cost += (d[end] + p[end]) * delta;

        for (int i = 0; i + 1 < path.size(); ++i) {
            int from = path[i], to = path[i + 1];
            g[from][to].c -= delta;
            g[to][from].c += delta;
        }

        d = Dijkstra(g, p, n, start);
        for (int v = 0; v < n + 1; ++v) {
            p[v] += d[v];
        }
    }

    return mp(cost, F);
}

void solve() {
    // task: H from https://codeforces.com/gym/100140

    int n, k;
    cin >> n >> k;

    // people 1..k, ships k+1...k+n
    const int start = 0, end = n + k + 1;

    vector<ll> capacity(n + k, 1);
    capacity.reserve(2 * (n + k));
    vector<int> cost(n + 1);
    for (int i = 1; i < n + 1; ++i) {
        cin >> capacity[k + i - 1] >> cost[i];
    }

    vector<Edge> edges;
    edges.reserve(2 * (n + k));

    for (int person = 1; person < k + 1; ++person) {
        edges.eb(start, person, 0);
    }
    for (int ship = k + 1; ship < n + k + 1; ++ship) {
        edges.eb(ship, end, 0);
    }

    for (int person = 1; person < k + 1; ++person) {
        edges.eb(person, start, 0);
        capacity.eb(0);
    }
    for (int ship = k + 1; ship < n + k + 1; ++ship) {
        edges.eb(end, ship, 0);
        capacity.eb(0);
    }

    for (int person = 1; person <= k; ++person) {
        int t;
        cin >> t;

        capacity.reserve(capacity.size() + 2 * t);
        edges.reserve(edges.size() + 2 * t);

        while (t--) {
            int ship;
            cin >> ship;
            edges.eb(person, ship + k, cost[ship]);
            capacity.eb(1);

            edges.eb(ship + k, person, -cost[ship]);
            capacity.eb(0);
        }
    }

    const auto [mn_cost, F] = MCMF(edges, capacity, n + k + 1, start, end);
    cout << ((F == k) ? mn_cost : -1) << endl;
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
