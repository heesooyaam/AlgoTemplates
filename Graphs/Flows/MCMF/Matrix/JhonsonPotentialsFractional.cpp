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

#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("fast-math")
#pragma GCC optimize("-O3")


template<typename TWeight, typename TFlow, template <typename...> class fb_edge>
class ford_bellman {
public:
    using T = TWeight;
    using K = TFlow;

    ford_bellman(const vector<fb_edge<T, K>>& edges, const int LARGEST_VERTEX, const int BEGIN, const double EPS = 1e-6)
    : MAX_D(numeric_limits<T>::max() - 1)
    , EPS(EPS)
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

                if (!c || (MAX_D - EPS <= d[from] && d[from] <= MAX_D + EPS)) continue;

                if (d[to] > d[from] + w) {
                    better = true;
                    d[to] = d[from] + w;
                }
            }
        }
    }

    const T MAX_D;
    const double EPS;
    vector<T> d;
};

namespace matrix {
    template<typename TWeight, template<typename...> class dijkstra_edge>
    class dijkstra_with_path {
    public:
        using T = TWeight;

        dijkstra_with_path(
            const vector<vector<dijkstra_edge<T>>>& g,
            const vector<T>& p, const int LARGEST_VERTEX,
            const int BEGIN, const int INDEXATION = 1,
            const double EPS = 1e-6
        )
        : MAX_D(numeric_limits<T>::max() - 1)
        , EPS(EPS)
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
        void do_dijkstra(const vector<vector<dijkstra_edge<T>>>& g, const vector<T>& p, const int LARGEST_VERTEX, const int INDEXATION) {
            vector<bool> used(LARGEST_VERTEX + 1);
            d[BEGIN] = 0;

            int nearest = BEGIN;
            while (true) {
                bool relaxed = false;

                auto min_d = MAX_D;
                for (int v = INDEXATION; v < LARGEST_VERTEX + 1; ++v) {
                    if (used[v] || (MAX_D - EPS <= d[v] && d[v] <= MAX_D + EPS) || d[v] >= min_d) continue;

                    relaxed = true;
                    min_d = d[v];
                    nearest = v;
                }

                if (!relaxed) break;
                used[nearest] = true;

                for (int to = INDEXATION; to < LARGEST_VERTEX + 1; ++to) {
                    if (used[to] || (-EPS <= g[nearest][to].c && g[nearest][to].c <= EPS)) continue;

                    if (const auto new_d = min_d - p[to] + g[nearest][to].w + p[nearest]; d[to] > new_d) {
                        d[to] = new_d;
                        prev[to] = nearest;
                    }
                }
            }
        }

        const T MAX_D;
        const double EPS;
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

    MCMF(const vector<TEdge<T, K>>& edges, const int LARGEST_VERTEX, const int BEGIN, const int END, const int INDEXATION = 1, const double EPS = 1e-6)
    : MAX_D(numeric_limits<T>::max() - 1)
    , MAX_F(numeric_limits<K>::max())
    , EPS(EPS)
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
            auto [d, path] = matrix::dijkstra_with_path(g, p, LARGEST_VERTEX, BEGIN, INDEXATION, EPS).as_pair(END);

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
    const double EPS;
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
    // task: https://codeforces.com/problemset/problem/277/E

    int n;
    cin >> n;
    vector<pair<int, int>> pts(n + 1, {-inf, -inf});
    int mx_y = -inf;
    for (int i = 1; i < n + 1; ++i) {
        cin >> pts[i].ff >> pts[i].ss;
        mx_y = max(mx_y, pts[i].ss);
    }

    int cnt = (int) count_if(all(pts), [mx_y](const auto& pt) {return pt.ss == mx_y;});
    if (cnt > 1) {
        cout << -1 << endl;
        return;
    }

    const int start = 0, end = 2 * n + 1;
    vector<int> i_to_o(n + 1);
    for (int i = 1; i < n + 1; ++i) {
        i_to_o[i] = n + i;
    }

    vector<Edge<float, int>> edges;

    auto root = find_if(all(pts), [mx_y](const auto& pt) {return pt.ss == mx_y;}) - pts.begin();
    MCMF<float, int, Edge>::add_edge(edges, start, i_to_o[root], 0, 1);
    for (int input_v = 1; input_v < n + 1; ++input_v) {
        MCMF<float, int, Edge>::add_edge(edges, start, input_v, 0, 2);
    }

    const auto dist = [](const auto& a, const auto& b) {
        return sqrtl((a.ff - b.ff) * (a.ff - b.ff) + (a.ss - b.ss) * (a.ss - b.ss));
    };

    for (int from = 1; from < n + 1; ++from) {
        for (int to = 1; to < n + 1; ++to) {
            if (pts[from].ss <= pts[to].ss) continue;

            MCMF<float, int, Edge>::add_edge(edges, from, i_to_o[to], dist(pts[from], pts[to]), 1);
        }
    }

    for (int v = 1; v < n + 1; ++v) {
        MCMF<float, int, Edge>::add_edge(edges, i_to_o[v], end, 0, 1);
    }

    const auto [cost, F] = MCMF(edges, 2 * n + 1, start, end, 0, 1e-6).as_pair();

    if (F == n) {
        cout << fixed << setprecision(10) << cost << endl;
    } else {
        cout << -1 << endl;
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
