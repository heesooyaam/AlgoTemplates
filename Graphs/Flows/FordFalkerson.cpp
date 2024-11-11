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

ll dfs(vector<unordered_map<int, ll>>& g, vector<int>& used, int cur, ll mnD, const int curColor) {
    auto isUsed = [&used, curColor] (int v) {
        return used[v] == curColor;
    };

    if (cur == g.size() - 1) return mnD;

    used[cur] = curColor;
    for (auto& [to, C] : g[cur]) {
        if (isUsed(to) || C == 0) continue;

        ll delta = dfs(g, used, to, min(mnD, C), curColor);
        if (delta > 0) {
            C -= delta;
            g[to][cur] += delta;
            return delta;
        }
    }
    return 0;
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

    vector<int> used(n + 1);
    int curColor = 1;
    ll F = 0;
    while (ll delta = dfs(g, used, 1, numeric_limits<ll>::max(), curColor)) {
        ++curColor;
        F += delta;
    }
    cout << F << endl;

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