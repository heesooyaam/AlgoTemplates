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

template<
        typename node_t,
        auto combine,
        auto neutral_node,
        typename push_t,
        auto composition, /* composition(old, new) */
        auto sync,
        auto neutral_push
        >
class seg_tree_t {
    static_assert(std::is_convertible<decltype(combine), std::function<node_t(node_t, node_t)>>());
    static_assert(std::is_convertible<decltype(composition), std::function<push_t(push_t, push_t)>>());
    static_assert(std::is_convertible<decltype(sync), std::function<node_t(node_t, push_t)>>());

    static_assert(std::is_convertible<decltype(neutral_node), std::function<node_t()>>());
    static_assert(std::is_convertible<decltype(neutral_push), std::function<push_t()>>());

    const int n;
    std::vector<node_t> tree;
    std::vector<push_t> lazy;

public:
    explicit seg_tree_t(int sz) : n(sz), tree(n << 2, neutral_node()), lazy(n << 2, neutral_push()) {}

    template<typename container_t>
    seg_tree_t(const container_t &data) : n(data.size()), tree(n << 2), lazy(n << 2, neutral_push()) {
        build(0, 0, n, data);
    }

    void update(const int l, const int r, const push_t &upd) {
        return update(0, 0, n, l, r, upd);
    }

    void update(const int pos, const push_t &upd) {
        return update(0, 0, n, pos, upd);
    }

    node_t get(const int l, const int r) {
        return get(0, 0, n, l, r);
    }

    node_t get(const int pos) {
        return get(0, 0, n, pos);
    }

private:
    template<typename container_t>
    void build(int idx, int cl, int cr, const container_t &data) {
        if (cl + 1 == cr) {
            tree[idx] = data[cl];
            return;
        }

        int mid = (cl + cr) >> 1;
        build((idx << 1) + 1, cl, mid, data);
        build((idx << 1) + 2, mid, cr, data);
        tree[idx] = combine(tree[(idx << 1) + 1], tree[(idx << 1) + 2]);
    }

    void update_node(int idx, const push_t &upd) {
        lazy[idx] = composition(lazy[idx], upd);
        tree[idx] = sync(tree[idx], upd);
    }

    void push(int idx) {
        update_node((idx << 1) + 1, lazy[idx]);
        update_node((idx << 1) + 2, lazy[idx]);

        lazy[idx] = neutral_push();
    }

    void update(int idx, int cl, int cr, int l, int r, const push_t &upd) {
        if (l >= cr || r <= cl) {
            return;
        }

        if (l <= cl && cr <= r) {
            update_node(idx, upd);
            return;
        }

        push(idx);

        int mid = (cl + cr) >> 1;
        update((idx << 1) + 1, cl, mid, l, r, upd);
        update((idx << 1) + 2, mid, cr, l, r, upd);
        tree[idx] = combine(tree[(idx << 1) + 1], tree[(idx << 1) + 2]);
    }

    void update(int idx, int cl, int cr, int pos, const push_t &upd) {
        if (cl + 1 == cr) {
            update_node(idx, upd);
            return;
        }

        push(idx);
        int mid = (cl + cr) >> 1;
        return pos < mid ?
            update((idx << 1) + 1, cl, mid, pos, upd) :
            update((idx << 1) + 2, mid, cr, pos, upd);
    }

    node_t get(int idx, int cl, int cr, int l, int r) {
        if (l >= cr || r <= cl) {
            return neutral_node();
        }

        if (l <= cl && cr <= r) {
            return tree[idx];
        }

        push(idx);

        int mid = (cl + cr) >> 1;
        return combine(
            get((idx << 1) + 1, cl, mid, l, r),
            get((idx << 1) + 2, mid, cr, l, r)
        );
    }

    node_t get(int idx, int cl, int cr, int pos) {
        if (cl + 1 == cr) {
            return tree[idx];
        }

        push(idx);
        int mid = (cl + cr) >> 1;
        return pos < mid ?
            get((idx << 1) + 1, cl, mid, pos) :
            get((idx << 1) + 2, mid, cr, pos);
    }
};

struct node_t { ll sum = 0, len = 1; };
using push_t = ll;

node_t combine(node_t a, node_t b) { return {a.sum + b.sum, a.len + b.len }; }
push_t composition(push_t old_p, push_t new_p) { return old_p + new_p; }
node_t sync(node_t x, push_t f) { return {x.sum += f * x.len, x.len}; }
node_t neutral_node() { return {0, 0}; }
push_t neutral_push() { return 0; }

void solve() {
    int n, m;
    cin >> n >> m;
    seg_tree_t<node_t, combine, neutral_node, push_t, composition, sync, neutral_push> ST(vector<node_t>(n, {0, 1}));
    for (int i = 0; i < m; ++i) {
        int op;
        cin >> op;
        if (op == 1) {
            int l, r, v;
            cin >> l >> r >> v;
            ST.update(l, r, v);
        } else {
            int l, r;
            cin >> l >> r;
            cout << ST.get(l, r).sum << endl;
        }
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
