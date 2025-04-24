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
#define print(x); for(auto& val : x){cout << val << ' ';} cout << endl;
#define print1(x); for (int i_ = 1; i_ < (x).size(); ++i_) {cout << (x)[i_] << ' ';} cout << endl;
#define input(x); for(auto& val : x){cin >> val;}
#define input1(x); for (int i_ = 1; i_ < (x).size(); ++i_) {cin >> (x)[i_];}
#define make_unique(x) sort(all((x))); (x).resize(unique(all((x))) - (x).begin())
#define endl '\n'

template<typename T = int64_t>
class max_add_tree_t {
public:
    struct node_t {
        T d = 0, mx;
        int pos;
    };

    explicit max_add_tree_t(int sz, T def = 0)
            : n(sz), tree(n << 2, {.mx = def}) {
        build(0, 0, n, vector<int>());
    }

    template<typename K>
    max_add_tree_t(const vector<K>& data)
            : n(data.size()), tree(n << 2)
    {
        static_assert(sizeof(T) >= sizeof(K));
        build(0, 0, n, data);
    }

    // {max, pos}
    pair<T, int> get_max(int l, int r) {
        return get_max(0, 0, n, l, r);
    }

    template<typename K>
    void update(int l, int r, K val) {
        static_assert(sizeof(T) >= sizeof(K));
        update(0, 0, n, l, r, val);
    }

    template<typename K>
    void update(int pos, K val) {
        static_assert(sizeof(T) >= sizeof(K));
        update(0, 0, n, pos, val);
    }

private:
    const int n;
    vector<node_t> tree;

    pair<T, int> merge(const pair<T, int>& a, const pair<T, int>& b, T d) const {
        return (a.ff >= b.ff ? mp(a.ff + d, a.ss) : mp(b.ff + d, b.ss));
    }

    template<typename K>
    void build(int idx, int curL, int curR, const vector<K>& data) {
        if (curL + 1 == curR) {
            tree[idx].mx = curL < data.size() ? data[curL] : tree[idx].mx;
            tree[idx].pos = curL;
            return;
        }

        int mid = (curL + curR) >> 1;

        build(idx * 2 + 1, curL, mid, data);
        build(idx * 2 + 2, mid, curR, data);

        tree[idx].mx = max(tree[idx * 2 + 1].mx, tree[idx * 2 + 2].mx) + tree[idx].d;
        tree[idx].pos = tree[idx].mx == tree[idx * 2 + 1].mx + tree[idx].d ? tree[idx * 2 + 1].pos : tree[idx * 2 + 2].pos;
    }

    pair<T, int> get_max(int idx, int curL, int curR, int l, int r) const {
        if (l >= r) {
            return {numeric_limits<T>::min(), -1};
        }

        if (curL == l && curR == r) {
            return mp(tree[idx].mx, tree[idx].pos);
        }

        int mid = (curL + curR) >> 1;

        return merge(get_max(idx * 2 + 1, curL, mid, l, min(r, mid)), get_max(idx * 2 + 2, mid, curR, max(l, mid), r), tree[idx].d);
    }

    template<typename K>
    void update(int idx, int curL, int curR, int l, int r, K val) {
        if (l >= r) {
            return;
        }

        if (curL == l && curR == r) {
            tree[idx].d += val;
            tree[idx].mx += val;
            return;
        }

        int mid = (curL + curR) >> 1;

        update(idx * 2 + 1, curL, mid, l, min(r, mid), val);
        update(idx * 2 + 2, mid, curR, max(l, mid), r, val);

        tree[idx].mx = max(tree[idx * 2 + 1].mx, tree[idx * 2 + 2].mx) + tree[idx].d;
        tree[idx].pos = tree[idx].mx == tree[idx * 2 + 1].mx + tree[idx].d ? tree[idx * 2 + 1].pos : tree[idx * 2 + 2].pos;
    }

    template<typename K>
    void update(int idx, int curL, int curR, int pos, K val) {
        if (curL + 1 == curR) {
            tree[idx].mx += val;
            return;
        }

        int mid = (curL + curR) >> 1;

        if (pos < mid) {
            update(idx * 2 + 1, curL, mid, pos, val);
        } else {
            update(idx * 2 + 2, mid, curR, pos, val);
        }

        tree[idx].mx = max(tree[idx * 2 + 1].mx, tree[idx * 2 + 2].mx) + tree[idx].d;
        tree[idx].pos = tree[idx].mx == tree[idx * 2 + 1].mx + tree[idx].d ? tree[idx * 2 + 1].pos : tree[idx * 2 + 2].pos;
    }
};

struct event_t {
    int l, r;
    int x;
    char what; // o - open, c - close
    bool operator<(const event_t& other) const {
        return (x == other.x ? what < other.what : x < other.x);
    }
};

int MAXX = 30'000;
void solve() {
    int a, b;
    cin >> a >> b;
    MAXX += b;
    int n;
    cin >> n;
    vector<event_t> events;
    events.reserve(2 * n);
    {
        vector<pair<int, int>> pts(n);
        for (auto &[x, y] : pts) {
            cin >> x >> y;
            y += MAXX;
        }

        for (auto &[x, y] : pts) {
            int l = y - b + 1;
            int r = y + b - 1;
            assert(l <= r);
            events.eb(l, r, x, 'o');
            events.eb(l, r, x + a, 'c');
        }
    }

    sort(all(events));
    max_add_tree_t<int> seg_tree(2 * MAXX + 1);

    int mx = 0;
    pair<int, int> pt;
    for (int i = 0; i < events.size(); ) {
        do {
            auto &[l, r, x, tp] = events[i++];
            seg_tree.update(l, r + 1, (tp == 'o' ? 1 : -1));
        } while (i < events.size() && events[i - 1].x == events[i].x);

        const auto [cur_mx, y] = seg_tree.get_max(0, 2 * MAXX + 1);
        if (cur_mx > mx) {
            mx = cur_mx;
            pt = mp(events[i - 1].x, y - MAXX);
        }
    }

    cout << mx << endl;
}

int32_t main() {
//    freopen("mine.in", "r", stdin);
//    freopen("mine.out", "w", stdout);
    ios::sync_with_stdio(0); cin.tie(0);
    int ttest = 1;
//    cin >> ttest;
    while(ttest--) solve();
    return 0;
}
