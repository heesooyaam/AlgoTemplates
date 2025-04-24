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
    };

    explicit max_add_tree_t(int sz, T def = 0) : n(sz), tree(n << 2, {.mx = def}) {}

    template<typename K>
    max_add_tree_t(const vector<K>& data)
            : n(data.size()), tree(n << 2)
    {
        static_assert(sizeof(T) >= sizeof(K));
        build(0, 0, n, data);
    }

    T get_max(int l, int r) {
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

    template<typename K>
    void build(int idx, int curL, int curR, const vector<K>& data) {
        if (curL + 1 == curR) {
            tree[idx].mx = data[curL];
            return;
        }

        int mid = (curL + curR) >> 1;

        build(idx * 2 + 1, curL, mid, data);
        build(idx * 2 + 2, mid, curR, data);

        tree[idx].mx = max(tree[idx * 2 + 1].mx, tree[idx * 2 + 2].mx);
    }

    T get_max(int idx, int curL, int curR, int l, int r) {
        if (l >= r) {
            return numeric_limits<T>::min();
        }

        if (curL == l && curR == r) {
            return tree[idx].mx;
        }

        int mid = (curL + curR) >> 1;

        return max(get_max(idx * 2 + 1, curL, mid, l, min(r, mid)), get_max(idx * 2 + 2, mid, curR, max(l, mid), r)) + tree[idx].d;
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

void solve() {
    int n;
    cin >> n;
    vector<array<pair<int, int>, 2>> pts(n);
    int mn_y = inf;
    int mx_y = -inf;
    for (auto &pr : pts) {
        for (auto &[x, y] : pr) {
            cin >> x >> y;
            mn_y = min(mn_y, y);
            mx_y = max(mx_y, y);
        }
    }

    if (mn_y < 0) {
        for (auto &pr : pts) {
            for (auto &[_, y] : pr) {
                y -= mn_y;
            }
        }
        mx_y -= mn_y;
    }

    vector<event_t> events;
    events.reserve(2 * n);
    for (auto &rec : pts) {
        int l = rec[0].ss;
        int r = rec[1].ss;
        events.eb(l, r, rec[0].ff, 'o');
        events.eb(l, r, rec[1].ff + 1, 'c');
    }

    sort(all(events));
    max_add_tree_t<int> seg_tree(mx_y + 1);

    int mx = 0;
    pair<int, int> pt;
    for (int i = 0; i < n; ) {
        do {
            auto &[l, r, x, c] = events[i++];
            seg_tree.update(l, r + 1, 1);
        } while (i < n && events[i - 1].x == events[i].x);
        const int cur_mx = seg_tree.get_max(0, mx_y + 1);
        if (cur_mx > mx) {
            mx = cur_mx;
            pt = mp(events[i - 1].x, events[i - 1].r + mn_y);
        }
    }
    cout << mx << endl;
    cout << pt.ff << ' ' << pt.ss << endl;
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
