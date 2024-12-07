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

template<typename TWeight, typename TFlow, template <typename...> class fb_edge>
class ford_bellman {
public:
    using T = TWeight;
    using K = TFlow;
    ford_bellman(const vector<fb_edge<T, K>>& edges, const int LARGEST_VERTEX, const int BEGIN)
    : MAX_D(numeric_limits<T>::max())
    , d(LARGEST_VERTEX + 1, MAX_D) {
        do_ford_bellman(edges, BEGIN);
    }

    const vector<T>& get_dist() const {
        return d;
    }
private:
    void do_ford_bellman(const vector<fb_edge<T, K>>& edges, const int BEGIN) {
        d[BEGIN] = 0;
        bool better = true;

        while (better) {
            better = false;
            for (const auto& edge : edges) {
                const auto from = edge.from, to = edge.to;
                const auto w = edge.w;
                const auto c = edge.c;

                if (!c || d[from] == MAX_D) continue;

                if (d[to] > (d[from] + w)) {
                    better = true;
                    d[to] = d[from] + w;
                }
            }
        }
    }

    const T MAX_D;
    vector<T> d;
};

namespace matrix {
    template<typename TWeight, template<typename...> class dijkstra_edge>
    class dijkstra_with_path {
    public:
        using T = TWeight;

        dijkstra_with_path(
            const vector<vector<dijkstra_edge<T>>>& g,
            const vector<T>& p,
            const int LARGEST_VERTEX,
            const int BEGIN,
            const int INDEXATION = 1
        )
        : MAX_D(numeric_limits<T>::max())
        , BEGIN(BEGIN)
        , d(LARGEST_VERTEX + 1, MAX_D)
        , prev(LARGEST_VERTEX + 1, -1) {
            do_dijkstra(g, p, LARGEST_VERTEX, INDEXATION);
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
        void do_dijkstra(
            const vector<vector<dijkstra_edge<T>>>& g,
            const vector<T>& p,
            const int LARGEST_VERTEX,
            const int INDEXATION
        ) {
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
                    if (used[to] || !g[nearest][to].c) continue;

                    if (const auto new_d = min_d - p[to] + g[nearest][to].w + p[nearest]; d[to] > new_d) {
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

template<typename TCost, typename TFlow, template<typename...> class TEdge>
class MCMF {
public:
    using T = TCost;
    using K = TFlow;

    template<typename W = T, typename F = K>
    struct d_edge {
        W w = -1;
        F c = 0;
    };

    MCMF(const vector<TEdge<T, K>>& edges, const int LARGEST_VERTEX, const int BEGIN, const int END, const int INDEXATION = 1)
    : MAX_D(numeric_limits<T>::max())
    , MAX_F(numeric_limits<K>::max())
    , F(0)
    , C(0) {
        do_mcmf(edges, LARGEST_VERTEX, BEGIN, END, INDEXATION);
    }
    
    T get_max_flow() const {
        return F;
    }
    
    K get_min_cost() const {
        return C;
    }
    
    pair<T, K> as_pair() const {
        return make_pair(C, F);
    }

    static void add_edge(vector<TEdge<T, K>>& edges, const int from, const int to, const T w, const K c) {
        edges.pb({.from = from, .to = to, .w = w, .c = c});
        edges.pb({.from = to, .to = from, .w = -w, .c = 0});
    }

private:
    void do_mcmf(const vector<TEdge<T, K>>& edges, const int LARGEST_VERTEX, const int BEGIN, const int END, const int INDEXATION) {
        const int M = edges.size();

        vector<vector<d_edge<T, K>>> g(LARGEST_VERTEX + 1, vector<d_edge<T, K>>(LARGEST_VERTEX + 1));
        for (const auto& edge : edges) {
            const auto from = edge.from, to = edge.to;
            const auto w = edge.w;
            const auto c = edge.c;

            g[from][to] = {w, c};
        }

        vector<T> p{ford_bellman(edges, LARGEST_VERTEX, BEGIN).get_dist()};

        while (true) {
            auto [d, path] = matrix::dijkstra_with_path(g, p, LARGEST_VERTEX, BEGIN, INDEXATION).as_pair(END);

            auto delta = MAX_F;
            for (int i = 0; i + 1 < path.size(); ++i) {
                int from = path[i], to = path[i + 1];
                delta = min(delta, g[from][to].c);
            }

            if (delta == MAX_F) {
                break;
            }

            F += delta;
            C += (d[END] + p[END]) * delta;

            for (int i = 0; i + 1 < path.size(); ++i) {
                int from = path[i], to = path[i + 1];
                g[from][to].c -= delta;
                g[to][from].c += delta;
            }

            for (int v = INDEXATION; v <= LARGEST_VERTEX; ++v) {
                p[v] += d[v];
            }
        }
    }

    const T MAX_D;
    const K MAX_F;
    TFlow F;
    TCost C;
};

template<typename TWeight, typename TFlow>
struct Edge {
    int from, to;
    TWeight w;
    TFlow c;
};

void solve() {
    // task: H from https://codeforces.com/gym/100140

    int n, k;
    cin >> n >> k;

    // people 1..k, ships k+1...k+n
    const int start = 0, end = n + k + 1;

    vector<int> capacity(n + 1);
    vector<int> cost(n + 1);

    for (int i = 1; i < n + 1; ++i) {
        cin >> capacity[i] >> cost[i];
    }

    vector<Edge<int, int>> edges;
    edges.reserve(2 * (n + k));

    for (int person = 1; person < k + 1; ++person) {
        MCMF<int, int, Edge>::add_edge(edges, start, person, 0, 1);
    }
    for (int ship = k + 1; ship < n + k + 1; ++ship) {
        MCMF<int, int, Edge>::add_edge(edges, ship, end, 0, capacity[ship - k]);
    }

    for (int person = 1; person <= k; ++person) {
        int t;
        cin >> t;

        while (t--) {
            int ship;
            cin >> ship;

            MCMF<int, int, Edge>::add_edge(edges, person, ship + k, cost[ship], 1);
        }
    }
    const auto [C, F] = MCMF(edges, n + k + 1, start, end, 0).as_pair();
    cerr << F << ' ' << C << endl;
    cout << ((F == k) ? C : -1) << endl;
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
