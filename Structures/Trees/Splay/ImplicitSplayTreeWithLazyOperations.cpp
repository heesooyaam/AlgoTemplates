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

template<typename value_t = int>
class implicit_splay_tree_t {
public:
    enum child_t : short { l_child = 0, r_child = 1 };
    struct node_t {
        struct data_t {
            value_t val = numeric_limits<value_t>::max();
            value_t d = 0;
            value_t mn = numeric_limits<value_t>::max();
            bool rev;
        };

        int parent = 0;
        int subtree_size = 0;
        data_t data{};
        array<int, 2> child{};

        int& operator[](short pos) {
            return child[pos];
        }
    };

    implicit_splay_tree_t() : root(), size(), tree() {
        tree.eb();
    }

private:
    int root;
    int size;
    vector<node_t> tree;
    node_t::data_t& get_node_data(const int node_idx) {
        return tree[node_idx].data;
    }

    void sync_changes(const int node_idx) {
        const int l_child_idx = tree[node_idx][l_child], r_child_idx = tree[node_idx][r_child];
        auto& node_data = get_node_data(node_idx);
        node_data.mn = node_data.val;
        tree[node_idx].subtree_size = 1;

        for (const int child_node_idx : {l_child_idx, r_child_idx}) {
            tree[node_idx].subtree_size += tree[child_node_idx].subtree_size;
            node_data.mn = min(node_data.mn, tree[child_node_idx].data.mn);
        }
    }

    void push(const int node_idx) {
        const int l_child_idx = tree[node_idx][l_child], r_child_idx = tree[node_idx][r_child];
        if (!node_idx || (l_child_idx + r_child_idx) == 0) {
            tree[node_idx].data.d = 0;
            tree[node_idx].data.rev += tree[node_idx].data.rev;
            return;
        }

        auto& node_data = get_node_data(node_idx);
        for (const int child_idx : {l_child_idx, r_child_idx}) {
            if (!child_idx) {
                continue;
            }

            auto& child_node_data = get_node_data(child_idx);
            child_node_data.rev += node_data.rev;
            child_node_data.d   += node_data.d;
            child_node_data.val += node_data.d;
            child_node_data.mn  += node_data.d;
        }

        node_data.d = 0;
        if (node_data.rev) {
            swap(tree[node_idx][l_child], tree[node_idx][r_child]);
            node_data.rev = false;
        }
    }

    void rotate(const int node_idx) {
        // parent of node_idx
        const int parent_idx = tree[node_idx].parent;

        assert(parent_idx);

        // tree[parent_idx][inv_child] == node_idx
        const child_t inv_child = tree[parent_idx][l_child] == node_idx ? l_child : r_child;

        // rerooting mid subtree
        const int b_subtree = tree[parent_idx][inv_child] = tree[node_idx][inv_child ^ 1];
        if (b_subtree) {
            tree[b_subtree].parent = parent_idx;
        }

        // process grandparent of node_idx
        const int gparent_idx = tree[node_idx].parent = tree[parent_idx].parent;
        if (gparent_idx) {
            (tree[gparent_idx][l_child] == parent_idx ? tree[gparent_idx][l_child] : tree[gparent_idx][r_child])
                    = node_idx;
        }

        // child became parent
        tree[parent_idx].parent = node_idx;
        tree[node_idx][inv_child ^ 1] = parent_idx;

        // updating subtree_size
//        push(parent_idx);
        sync_changes(parent_idx);
        push(node_idx); // !!!!!! TODO
        sync_changes(node_idx);
    }

    inline void zig(const int node_idx) {
        rotate(node_idx);
    }

    inline void zigzig(const int node_idx) {
        rotate(tree[node_idx].parent);
        rotate(node_idx);
    }

    inline void zigzag(const int node_idx) {
        rotate(node_idx);
        rotate(node_idx);
    }

    void splay(const int node_idx) {
        while (tree[node_idx].parent) {
            const int parent_idx = tree[node_idx].parent;
            const int gparent_idx = tree[parent_idx].parent;
            if (!gparent_idx) {
                zig(node_idx);
            } else if (
                    (tree[gparent_idx][l_child] == parent_idx)
                    == (tree[parent_idx][l_child] == node_idx)
                    ) {
                zigzig(node_idx);
            } else {
                zigzag(node_idx);
            }
        }
    }

    int get_subtree_size(int node_idx, child_t child) {
        return tree[tree[node_idx][child]].subtree_size;
    }

    // makes node on position pos the root of its tree
    int find(int node_idx, const int pos) {
        int node_pos = 0;
        while (true) {
            push(node_idx);
            sync_changes(node_idx);
            const int l_subtree_size = get_subtree_size(node_idx, l_child);
            if (node_pos + l_subtree_size == pos) {
                splay(node_idx);
                return node_idx;
            } else if (node_pos + l_subtree_size < pos) {
                node_pos += l_subtree_size + 1;
                node_idx = tree[node_idx][r_child];
            } else {
                node_idx = tree[node_idx][l_child];
            }
        }

        assert(false);
    }

    // pos(l_tree) = pos
    pair<int, int> split(int node_idx, const int pos) {
        if (!node_idx) {
            return {0, 0};
        }

        if (pos == -1) {
            return {0, node_idx};
        }

        const int l_root = find(node_idx, pos);
        const int r_root = tree[l_root][r_child];

        if (r_root) {
            tree[r_root].parent = 0;
            tree[l_root][r_child] = 0;
        }

        push(l_root);
        sync_changes(l_root);

        return {l_root, r_root};
    }

    int merge(int l_tree, int r_tree) {
        if (!l_tree) {
            return r_tree;
        }

        if (!r_tree) {
            return l_tree;
        }

        l_tree = find(l_tree, tree[l_tree].subtree_size - 1);
        tree[l_tree][r_child] = r_tree;
        tree[r_tree].parent   = l_tree;

        push(l_tree);
        sync_changes(l_tree);

        return l_tree;
    }

    int create_node() {
        tree.eb();
        return tree.size() - 1;
    }

    int insert_after(int node_idx, int pos, value_t val) {
        if (pos < 0) {
            int new_node_idx = create_node();
            tree[new_node_idx].subtree_size = 1;
            tree[new_node_idx].data.val = val;
            return merge(new_node_idx, node_idx);
        }

        auto [l_tree, r_tree] = split(node_idx, pos);
        int new_node_idx = create_node();

        tree[new_node_idx].parent = l_tree;
        tree[new_node_idx].subtree_size = 1;
        tree[new_node_idx].data.val = val;

        tree[l_tree][r_child] = new_node_idx;
        push(l_tree);
        sync_changes(l_tree);

        splay(l_tree = new_node_idx);
        return merge(l_tree, r_tree);
    }

    int erase(int node_idx, int pos) {
        auto [l_tree, r_tree] = split(node_idx, pos);

        l_tree = tree[l_tree][l_child];
        tree[l_tree].parent = 0;

        return merge(l_tree, r_tree);
    }

public:
    void insert_after(int pos, value_t val) {
        if (pos < -1 || pos >= size) {
            return;
        }

        root = insert_after(root, pos, val);
        ++size;
    }

    int get(int pos) {
        root = find(root, pos);
        return tree[root].data.val;
    }

    void erase(int pos) {
        if (pos < 0 || pos >= size) {
            return;
        }

        root = erase(root, pos);
        --size;
    }

    // [l, r)
    void add(int l, int r, value_t x) {
        auto [pref, bad] = split(root, r - 1);
        auto [bad2, need] = split(pref, l - 1);

        auto& need_data = get_node_data(need);
        need_data.mn  += x;
        need_data.val += x;
        need_data.d   += x;
        push(need);
        sync_changes(need);
        root = merge(merge(bad2, need), bad);
    }

    void reverse(int l, int r) {
        auto [pref, bad] = split(root, r - 1);
        auto [bad2, need] = split(pref, l - 1);

        get_node_data(need).rev += 1;
        push(need);
        sync_changes(need);

        root = merge(merge(bad2, need), bad);
    }

    value_t get_min(int l, int r) {
        auto [pref, bad] = split(root, r - 1);
        auto [bad2, need] = split(pref, l - 1);

        const value_t ret = get_node_data(need).mn;
        root = merge(merge(bad2, need), bad);

        return ret;
    }

    void revolve(int l, int r, int t) {
        auto [pref, bad] = split(root, r - 1);
        auto [bad2, need] = split(pref, l - 1);
        auto [half1, half2] = split(need, r - l + 1 - t);

        root = merge(merge(bad2, merge(half2, half1)), bad);
    }
};

// it does not work :(

void solve() {
    int n;
    cin >> n;
    implicit_splay_tree_t<ll> tr;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        tr.insert_after(i - 1, x);
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        string s;
        cin >> s;
        int l, r;
        if (s == "ADD") {
            ll x;
            cin >> l >> r;
            cin >> x;
            tr.add(l - 1, r, x);
        } else if (s == "REVERSE") {
            cin >> l >> r;
            tr.reverse(l - 1, r);
        } else if (s == "REVOLVE") {
            int t;
            cin >> l >> r;
            cin >> t;
            t %= (r - l + 1);
            tr.revolve(l - 1, r, t);
        } else if (s == "INSERT") {
            ll pos, x;
            cin >> pos >> x;
            tr.insert_after(pos - 1, x);
        } else if (s == "DELETE") {
            ll pos;
            cin >> pos;
            tr.erase(pos - 1);
        } else {
            cin >> l >> r;
            cout << tr.get_min(l - 1, r) << endl;
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
