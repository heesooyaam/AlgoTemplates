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
    ll w;
};

vector<ll> FordBellman(const vector<Edge>& edges, int n, const int start) {
    vector<ll> d(n + 1, INF);
    d[start] = 0;
    bool better = true;

    while (n-- && better) {
        better = false;
        for (const auto& [from, to, w] : edges) {
            if (d[from] != inf && d[to] > d[from] + w) {
                better = true;
                d[to] = d[from] + w;
            }
        }
    }

    return d;
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (auto& [from, to, w] : edges) {
        cin >> from >> to >> w;
    }
    auto d{FordBellman(edges, n, 1)};
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
