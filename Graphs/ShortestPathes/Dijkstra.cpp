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
    using DijkstraEdge = ll;

    // if there is no edge (u, v) let g[u][v] be -1

    inline pair<vector<ll>, vector<int>> DijkstraWithPath(const vector<vector<DijkstraEdge>>& g, const int n, const int start, const int end = -1) {
        vector<bool> used(n + 1);
        vector<int> prv(n + 1, -1);

        vector<ll> d(n + 1, INF);
        d[start] = 0;

        int nearest = start;
        while (true) {
            bool relaxed = false;

            ll min_d = INF;
            for (int v = 1; v < n + 1; ++v) {
                if (used[v] || d[v] == INF || d[v] >= min_d) continue;

                relaxed = true;
                min_d = d[v];
                nearest = v;
            }

            if (!relaxed) break;
            used[nearest] = true;

            for (int to = 1; to < n + 1; ++to) {
                if (used[to] || g[nearest][to] == -1) continue;

                if (ll new_d = min_d + g[nearest][to]; d[to] > new_d) {
                    d[to] = new_d;
                    prv[to] = nearest;
                }
            }
        }

        return mp(d, (end == -1) ? prv : GetPath(prv, start, end));
    }

    inline vector<ll> Dijkstra(const vector<vector<DijkstraEdge>>& g, const int n, const int start) {
        vector<bool> used(n + 1);

        vector<ll> d(n + 1, INF);
        d[start] = 0;

        int nearest = start;
        while (true) {
            bool relaxed = false;

            ll min_d = INF;
            for (int v = 1; v < n + 1; ++v) {
                if (used[v] || d[v] == INF || d[v] >= min_d) continue;

                relaxed = true;
                min_d = d[v];
                nearest = v;
            }

            if (!relaxed) break;
            used[nearest] = true;

            for (int to = 1; to < g[nearest].size(); ++to) {
                if (used[to] || g[nearest][to] == -1) continue;

                d[to] = min(d[to], d[nearest] + g[nearest][to]);
            }
        }

        return d;
    }
} // namespace Matrix

namespace List {
    // create struct called DijkstraEdge if u wanna store additional info (sample: Graphs/Flows/MCMF/JhonsonPotentials.cpp),
    // but do not forget to change for ([to, w] : g[nearest]) {...} then
    using DijkstraEdge = pair<int, ll>; // {to, weight}

    inline pair<vector<ll>, vector<int>> SparseDijkstraWithPath(const vector<vector<DijkstraEdge>>& g, const int n, const int start, const int end = -1) {
        vector<bool> used(n + 1);
        vector<int> prv(n + 1, -1);
        vector<ll> d(n + 1, INF);
        d[start] = 0;

        set<pair<ll, int>> dist_st{mp(d[start], start)};
        while (!dist_st.empty()) {
            const auto [nearest_d, nearest] = *dist_st.begin();
            dist_st.erase(dist_st.begin());
            used[nearest] = true;

            for (const auto& [to, w] : g[nearest]) {
                ll new_d = nearest_d + w;
                if (used[to] || new_d >= d[to]) continue;

                auto it = dist_st.find(mp(d[to], to));
                if (it != dist_st.end()) {
                    dist_st.erase(mp(d[to], to));
                }
                dist_st.emplace(new_d, to);
                prv[to] = nearest;
                d[to] = new_d;
            }
        }

        return mp(d, (end == -1) ? prv : GetPath(prv, start, end));
    }

    inline pair<vector<ll>, vector<int>> DenseDijkstraWithPath(const vector<vector<DijkstraEdge>>& g, const int n, const int start, const int end = -1) {
        vector<bool> used(n + 1);
        vector<int> prv(n + 1, -1);

        vector<ll> d(n + 1, INF);
        d[start] = 0;

        int nearest = start;
        while (true) {
            bool relaxed = false;

            ll min_d = INF;
            for (int v = 1; v < n + 1; ++v) {
                if (used[v] || d[v] == INF || d[v] >= min_d) continue;

                relaxed = true;
                min_d = d[v];
                nearest = v;
            }

            if (!relaxed) break;
            used[nearest] = true;

            for (const auto& [to, w] : g[nearest]) {
                if (used[to]) continue;

                if (ll new_d = min_d + w; d[to] > new_d) {
                    d[to] = new_d;
                    prv[to] = nearest;
                }
            }
        }

        return mp(d, (end == -1) ? prv : GetPath(prv, start, end));
    }

    inline pair<vector<ll>, vector<int>> DijkstraWithPath(
        const vector<vector<DijkstraEdge>>& g,
        const int n,
        const int m,
        const int start,
        const int end = -1
    ) {
        if (1ll * n * n < 1ll * m * (ll) log2(n)) {
            // do not forget to change on 0-indexation
            return DenseDijkstraWithPath(g, n, start, end);
        } else {
            return SparseDijkstraWithPath(g, n, start, end);
        }
    }

    inline vector<ll> SparseDijkstra(const vector<vector<DijkstraEdge>>& g, const int n, const int start) {
        vector<ll> d(n + 1, INF);
        vector<bool> used(n + 1);
        d[start] = 0;

        set<pair<ll, int>> dist_st{mp(d[start], start)};
        while (!dist_st.empty()) {
            const auto [nearest_d, nearest] = *dist_st.begin();
            dist_st.erase(dist_st.begin());
            used[nearest] = true;

            for (const auto& [to, w] : g[nearest]) {
                ll new_d = nearest_d + w;
                if (used[to] || new_d >= d[to]) continue;

                auto it = dist_st.find(mp(d[to], to));
                if (it != dist_st.end()) {
                    dist_st.erase(mp(d[to], to));
                }
                dist_st.emplace(new_d, to);
                d[to] = new_d;
            }
        }

        return d;
    }

    inline vector<ll> DenseDijkstra(const vector<vector<DijkstraEdge>>& g, const int n, const int start) {
        vector<bool> used(n + 1);

        vector<ll> d(n + 1, INF);
        d[start] = 0;

        int nearest = start;
        while (true) {
            bool relaxed = false;

            ll min_d = INF;
            for (int v = 1; v < n + 1; ++v) {
                if (used[v] || d[v] == INF || d[v] >= min_d) continue;

                relaxed = true;
                min_d = d[v];
                nearest = v;
            }

            if (!relaxed) break;
            used[nearest] = true;

            for (const auto& [to, w] : g[nearest]) {
                if (used[to]) continue;

                d[to] = min(d[to], d[nearest] + w);
            }
        }

        return d;
    }

    inline vector<ll> Dijkstra(
        const vector<vector<DijkstraEdge>>& g,
        const int n,
        const int m,
        const int start
    ) {
        if (1ll * n * n < 1ll * m * (ll) log2(n)) {
            // do not forget to change on 0-indexation
            return DenseDijkstra(g, n, start);
        } else {
            return SparseDijkstra(g, n, start);
        }
    }
} // namespace List

void solve() {
    // easy tasks to check ur changings: https://informatics.msk.ru/course/view.php?id=6#section-4
    int n, start, end;
    cin >> n >> start >> end;
    vector<vector<Matrix::DijkstraEdge>> g(n + 1, vector<Matrix::DijkstraEdge>(n + 1));
    for (int from = 1; from < n + 1; ++from) {
        for (int to = 1; to < n + 1; ++to) {
            cin >> g[from][to];
        }
    }
    const auto [_, path] = Matrix::DijkstraWithPath(g, n, start, end);
    if (path.empty()) {
        cout << -1 << endl;
    } else {
        print(path);
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
