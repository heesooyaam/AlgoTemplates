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

namespace matrix {
    // create struct called dijkstra_edge if u wanna store additional info (sample: Graphs/Flows/MCMF/JhonsonPotentials.cpp)
    // if there is no edge (u, v) let g[u][v].w be -1
    template<typename TWeight>
    struct d_edge {
        TWeight w;
    };
    
    template<typename TWeight, template<typename...> class dijkstra_edge>
    class dijkstra {
    public:
        using T = TWeight;
        
        dijkstra(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int BEGIN, const int INDEXATION = 1) 
        : MAX_D(numeric_limits<T>::max())
        , d(LARGEST_VERTEX + 1, MAX_D){
            do_dijkstra(g, LARGEST_VERTEX, BEGIN, INDEXATION);
        }

        const vector<T>& get_dist() const {
            return d;
        }
        
    private:
        void do_dijkstra(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int BEGIN, const int INDEXATION) {
            vector<bool> used(LARGEST_VERTEX + 1);
            d[BEGIN] = 0;

            int nearest = BEGIN;
            while (true) {
                bool relaxed = false;

                auto min_d = MAX_D;
                for (int v = INDEXATION; v < LARGEST_VERTEX + 1; ++v) {
                    if (used[v] || d[v] == MAX_D || d[v] >= min_d) continue;

                    relaxed = true;
                    min_d = d[v];
                    nearest = v;
                }

                if (!relaxed) break;
                used[nearest] = true;

                for (int to = INDEXATION; to < g[nearest].size(); ++to) {
                    if (used[to] || g[nearest][to].w == -1) continue;

                    d[to] = min(d[to], d[nearest] + g[nearest][to].w);
                }
            }
        }
        
        const T MAX_D;
        vector<T> d;
    };

    template<typename TWeight, template<typename...> class dijkstra_edge>
    class dijkstra_with_path {
    public:
        using T = TWeight;

        dijkstra_with_path(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int BEGIN, const int INDEXATION = 1)
        : MAX_D(numeric_limits<T>::max())
        , BEGIN(BEGIN)
        , d(LARGEST_VERTEX + 1, MAX_D)
        , prev(LARGEST_VERTEX + 1, -1) {
            do_dijkstra(g, LARGEST_VERTEX, INDEXATION);
        }

        vector<int> get_path(int target) const {
            vector<int> path;
            while (target != -1) {
                path.eb(target);
                target = prev[target];
            }

            if (path.back() != BEGIN) {
                return {};
            }

            reverse(all(path));
            return path;
        }

        const vector<T>& get_dist() const {
            return d;
        }

        const vector<int>& get_prev() const {
            return prev;
        }
        
        pair<vector<T>, vector<int>> as_pair(const int target = -1) const {
            return make_pair(d, (target == -1) ? prev : get_path(target));
        }

    private:
        void do_dijkstra(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int INDEXATION) {
            vector<bool> used(LARGEST_VERTEX + 1);
            d[BEGIN] = 0;

            int nearest = BEGIN;
            while (true) {
                bool relaxed = false;

                auto min_d = MAX_D;
                for (int v = INDEXATION; v < LARGEST_VERTEX + 1; ++v) {
                    if (used[v] || d[v] == MAX_D || d[v] >= min_d) continue;

                    relaxed = true;
                    min_d = d[v];
                    nearest = v;
                }

                if (!relaxed) break;
                used[nearest] = true;

                for (int to = INDEXATION; to < LARGEST_VERTEX + 1; ++to) {
                    if (used[to] || g[nearest][to].w == -1) continue;

                    if (const auto new_d = min_d + g[nearest][to].w; d[to] > new_d) {
                        d[to] = new_d;
                        prev[to] = nearest;
                    }
                }
            }
        }

        const T MAX_D;
        const int BEGIN;
        vector<T> d;
        vector<int> prev;
    };
} // namespace matrix


namespace List {
    // edit "dijkstra_edge",
    // if u wanna store additional info
    // (sample: Graphs/Flows/MCMF/JhonsonPotentials.cpp)

    template<typename TWeight>
    struct d_edge {
        int to;
        TWeight w;
    };
    
    template<typename TWeight, template<typename...> class dijkstra_edge>
    class dijkstra {
    public:
        using T = TWeight;

        dijkstra(
            const vector<vector<dijkstra_edge<T>>>& g,
            const int LARGEST_VERTEX,
            const int M,
            const int BEGIN,
            const int INDEXATION = 1
        )
        : MAX_D(numeric_limits<T>::max())
        , d(LARGEST_VERTEX + 1, MAX_D) {
            if (1ll * LARGEST_VERTEX * LARGEST_VERTEX < 1ll * M * (ll) log2(LARGEST_VERTEX)) {
                dense_dijkstra(g, LARGEST_VERTEX, BEGIN, INDEXATION);
            } else {
                sparse_dijkstra(g, LARGEST_VERTEX, BEGIN);
            }
        }

        const vector<T>& get_dist() const {
            return d;
        }
    private:
        inline void dense_dijkstra(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int BEGIN, const int INDEXATION) {
            vector<bool> used(LARGEST_VERTEX + 1);
            fill(all(d), MAX_D);
            d[BEGIN] = 0;

            int nearest = BEGIN;
            while (true) {
                bool relaxed = false;

                auto min_d = MAX_D;
                for (int v = INDEXATION; v < LARGEST_VERTEX + 1; ++v) {
                    if (used[v] || d[v] == MAX_D || d[v] >= min_d) continue;

                    relaxed = true;
                    min_d = d[v];
                    nearest = v;
                }

                if (!relaxed) break;
                used[nearest] = true;

                for (const auto& edge : g[nearest]) {
                    const auto to = edge.to;
                    const auto w = edge.w;

                    if (used[to]) continue;

                    d[to] = min(d[to], d[nearest] + w);
                }
            }
        }

        inline void sparse_dijkstra(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int BEGIN) {
            vector<bool> used(LARGEST_VERTEX + 1);
            d[BEGIN] = 0;

            set<pair<T, int>> dist_st{make_pair(d[BEGIN], BEGIN)};
            while (!dist_st.empty()) {
                const auto [nearest_d, nearest] = *dist_st.begin();
                dist_st.erase(dist_st.begin());
                used[nearest] = true;

                for (const auto& edge : g[nearest]) {
                    const auto to = edge.to;
                    const auto w = edge.w;

                    T new_d = nearest_d + w;
                    if (used[to] || new_d >= d[to]) continue;

                    auto it = dist_st.find(make_pair(d[to], to));
                    if (it != dist_st.END()) {
                        dist_st.erase(make_pair(d[to], to));
                    }
                    dist_st.emplace(new_d, to);
                    d[to] = new_d;
                }
            }
        }

        const T MAX_D;
        vector<T> d;
    };

    template<typename TWeight, template<typename...> class dijkstra_edge>
    class dijkstra_with_path {
    public:
        using T = TWeight;

        dijkstra_with_path(
            const vector<vector<dijkstra_edge<T>>>& g,
            const int LARGEST_VERTEX,
            const int M,
            const int BEGIN,
            const int INDEXATION = 1
        )
        : MAX_D(numeric_limits<T>::max())
        , BEGIN(BEGIN)
        , d(LARGEST_VERTEX + 1, MAX_D)
        , prev(LARGEST_VERTEX + 1, -1) {
            if (1ll * LARGEST_VERTEX * LARGEST_VERTEX < 1ll * M * (ll) log2(LARGEST_VERTEX)) {
                dense_dijkstra_with_path(g, LARGEST_VERTEX, INDEXATION);
            } else {
                sparse_dijkstra_with_path(g, LARGEST_VERTEX);
            }
        }

        vector<int> get_path(int target) const {
            vector<int> path;
            while (target != -1) {
                path.eb(target);
                target = prev[target];
            }
            if (path.back() != BEGIN) {
                return {};
            }

            reverse(all(path));
            return path;
        }

        const vector<T>& get_dist() const {
            return d;
        }

        const vector<T>& get_prev() const {
            return prev;
        }

        pair<vector<T>, vector<int>> as_pair(const int target = -1) const {
            return make_pair(d, (target == -1) ? prev : get_path(target));
        }

    private:
        void dense_dijkstra_with_path(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX, const int INDEXATION) {
            vector<bool> used(LARGEST_VERTEX + 1);
            d[BEGIN] = 0;

            int nearest = BEGIN;
            while (true) {
                bool relaxed = false;

                auto min_d = MAX_D;
                for (int v = INDEXATION; v < LARGEST_VERTEX + 1; ++v) {
                    if (used[v] || d[v] == MAX_D || d[v] >= min_d) continue;

                    relaxed = true;
                    min_d = d[v];
                    nearest = v;
                }

                if (!relaxed) break;
                used[nearest] = true;

                for (const auto& edge : g[nearest]) {
                    const auto to = edge.to;
                    const auto w = edge.w;

                    if (used[to] || d[to] >= d[nearest] + w) continue;

                    prev[to] = nearest;
                    d[to] = d[nearest] + w;
                }
            }
        }

        void sparse_dijkstra_with_path(const vector<vector<dijkstra_edge<T>>>& g, const int LARGEST_VERTEX) {
            vector<bool> used(LARGEST_VERTEX + 1);
            d[BEGIN] = 0;

            set<pair<ll, int>> dist_st{make_pair(d[BEGIN], BEGIN)};
            while (!dist_st.empty()) {
                const auto [nearest_d, nearest] = *dist_st.begin();
                dist_st.erase(dist_st.begin());
                used[nearest] = true;

                for (const auto& edge : g[nearest]) {
                    const auto to = edge.to;
                    const auto w = edge.w;
                    const auto new_d = nearest_d + w;

                    if (used[to] || new_d >= d[to]) continue;

                    auto it = dist_st.find(make_pair(d[to], to));
                    if (it != dist_st.end()) {
                        dist_st.erase(make_pair(d[to], to));
                    }
                    dist_st.emplace(new_d, to);
                    prev[to] = nearest;
                    d[to] = new_d;
                }
            }
        }

        const T MAX_D;
        const int BEGIN;
        vector<T> d;
        vector<int> prev;
    };
}

using namespace matrix;
void solve() {
    // easy tasks to check ur changings: https://informatics.msk.ru/course/view.php?id=6#section-4

    int n, start, end;
    cin >> n >> start >> end;
    vector<vector<d_edge<ll>>> g(n + 1, vector<d_edge<ll>>(n + 1));
    for (int from = 1; from < n + 1; ++from) {
        for (int to = 1; to < n + 1; ++to) {
            cin >> g[from][to].w;
        }
    }
    const auto& path = dijkstra_with_path(g, n, start).get_path(end);
    if (path.empty()) {
        cout << -1 << endl;
    } else {
        print(path);
    }

    cout << numeric_limits<double>::max() + 1 << endl;
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
