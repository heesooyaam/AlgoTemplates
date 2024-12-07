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

// example of edge class
template<typename T = int64_t>
struct Edge {
    int from, to;
    T w;
};

template<typename T, template<typename...> class fb_edge>
class ford_bellman {
public:
    ford_bellman(const vector<fb_edge<T>>& edges, const int LARGEST_VERTEX, const int BEGIN, const T EPS = 1e-6)
    : MAX_D(numeric_limits<T>::max() - 1)
    , EPS(EPS)
    , d(LARGEST_VERTEX + 1, MAX_D) {
        do_ford_bellman(edges, BEGIN);
    }
    const vector<T>& get_dist() const {
        return d;
    }
private:
    void do_ford_bellman(const vector<fb_edge<T>>& edges, const int BEGIN) {
        d[BEGIN] = 0;
        bool better = true;

        while (better) {
            better = false;
            for (const auto& edge : edges) {
                const auto from = edge.from, to = edge.to;
                const auto w = edge.w;

                if (!(MAX_D - EPS <= d[from] && d[from] <= MAX_D + EPS) && d[to] > d[from] + w) {
                    better = true;
                    d[to] = d[from] + w;
                }
            }
        }
    }

    const T MAX_D;
    const T EPS;
    vector<T> d;
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<Edge<int>> edges(m);
    for (auto& [from, to, w] : edges) {
        cin >> from >> to >> w;
    }
    auto d{ford_bellman(edges, n, 1).get_dist()};
    for (int i = 1; i < d.size(); ++i) {
        cout << ((d[i] == inf) ? 30000 : d[i]) << ' ';
    }
    cout << endl;
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
