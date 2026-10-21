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

#define int ll

class traverser;

class generalized_suffix_automaton {
    using ui = uint32_t;
public:
    struct node {
        ui get_next(char c) const {
            return go[c - 'a'];
        }
        
        void set_next(char c, ui nodeid) {
            go[c - 'a'] = nodeid;
        }

    public:
        ui sufflink = 0;
        ui len = 0;
        ui occ = 0;
        ll cost = 0;

    private:
        array<ui, 26> go{};
    };

private:
    template<typename ...T>
    ui create(T&&... x) {
        st.emplace_back(std::forward<T>(x)...);
        return st.size() - 1;
    }

    ui split(ui v, ui q, char c) {
        const ui qclone = create(get(q));
        get(qclone).len = get(v).len + 1;
        get(q).sufflink = qclone;
        
        // do not forget to reset such fields to default
        get(qclone).occ = 0;
        get(qclone).cost = 0;

        while (v && get(v).get_next(c) == q) {
            get(v).set_next(c, qclone);
            v = get(v).sufflink;
        }

        return qclone;
    }

    void push_back(char c, ll cost) {
        ui v = last;
        if (get(v).get_next(c)) {
            const ui q = get(v).get_next(c);
            last = get(v).len + 1 == get(q).len ? q : split(v, q, c);
        } else {
            const ui newstate = create();
            get(newstate).len = get(v).len + 1;
            while (v && !get(v).get_next(c)) {
                get(v).set_next(c, newstate);
                v = get(v).sufflink;
            }

            if (!v) {
                get(newstate).sufflink = root;
            } else {
                const ui q = get(v).get_next(c);
                get(newstate).sufflink = get(v).len + 1 == get(q).len ? q : split(v, q, c);
            }
            last = newstate;
        }
        ++get(last).occ;
        get(last).cost += cost;
    }

public:
    generalized_suffix_automaton()
    {
        create();
        create();
    }

    void add(string_view s, ll cost) {
        last = root;
        for (auto c : s) {
            push_back(c, cost);
        }
    }

    void count_occurrences() {
        vector<ui> order(st.size() - 2);
        iota(all(order), 2);
        sort(all(order), [&](ui a, ui b) {
            return get(a).len > get(b).len;
        });

        for (ui v : order) {
            sufflink(v).occ += get(v).occ;
        }

        get(root).occ = 0;
    }

    void count_costs() {
        vector<ui> order(st.size() - 2);
        iota(all(order), 2);
        sort(all(order), [&](ui a, ui b) {
            return get(a).len > get(b).len;
        });

        for (ui v : order) {
            sufflink(v).cost += get(v).cost;
        }

        get(root).cost = 0;
    }

    node& get(ui v) {
        return st[v];
    }

    node& next(ui v, char c) {
        return st[get(v).get_next(c)];
    }

    node& sufflink(ui v) {
        return get(get(v).sufflink);
    }

    pair<ui, ui> get_state_size(ui v) {
        if (v == root) {
            return {0, 0};
        }
        return {sufflink(v).len + 1, get(v).len};
    }

    size_t states_amount() const {
        return st.size();
    }

private:
    friend class traverser;
    static constexpr ui root = 1;
    ui last = root;
    vector<node> st;
};

class traverser {
public:
    using ui = uint32_t;
    using node = generalized_suffix_automaton::node;

    explicit traverser(generalized_suffix_automaton& sa, ui start_state = generalized_suffix_automaton::root)
        : cur(start_state)
        , len(sa.get(cur).len)
        , sa(sa)
    {}

    traverser(const traverser&) = default;

    bool can_move_by_char(char c) {
        return sa.get(cur).get_next(c);
    }

    void move_by_char(char c) {
        ++len;
        cur = sa.get(cur).get_next(c);
    }

    void move_to_root() {
        len = 0;
        cur = generalized_suffix_automaton::root;
    }

    const node& get_next(char c) {
        return sa.get(sa.get(cur).get_next(c));
    }

    const node& get_sufflink() {
        return sa.sufflink(cur);
    }

    bool can_move_by_sufflink() {
        return sa.get(cur).sufflink;
    }

    void move_by_sufflink() {
        cur = sa.get(cur).sufflink;
        len = sa.get(cur).len;
    }

    // length must be <= len
    void move_to_suffix(ui length) {
        while (can_move_by_sufflink() && get_sufflink().len >= length) {
            move_by_sufflink();
        }
        len = length;
    }

    void pop_front() {
        move_to_suffix(len - 1);
    }

    const node& get_current_state() {
        return sa.get(cur);
    }

    ui get_current_state_id() {
        return cur;
    }

    ui get_length() const {
        return len;
    }

private:
    ui cur;
    ui len = 0;
    generalized_suffix_automaton& sa;
};

// task: https://codeforces.com/contest/616/problem/F

void solve() {
    int n;
    cin >> n;
    generalized_suffix_automaton sa;
    vector<string> strs(n);
    input(strs);
    for (int i = 0; i < n; ++i) {
        ll c;
        cin >> c;
        sa.add(strs[i], c);
    }

    sa.count_costs();
    
    traverser tr(sa);
    vector<bool> used(sa.states_amount());
    ll ans = 0;
    auto dfs = [&](auto&& self, traverser tr) -> void {
        used[tr.get_current_state_id()] = true;
        if (tr.get_current_state().cost > 0) {
            ans = max(ans, tr.get_current_state().len * tr.get_current_state().cost);
        }

        for (char c = 'a'; c <= 'z'; ++c) {
            if (tr.can_move_by_char(c) && !used[tr.get_current_state().get_next(c)]) {
                auto cptr = tr;
                cptr.move_by_char(c);
                self(self, cptr);
            }
        }
    };

    dfs(dfs, tr);
    cout << ans << endl;
}

int32_t main() {
    // freopen("adjustment.in", "r", stdin);
    // freopen("adjustment.out", "w", stdout);
    ios::sync_with_stdio(0); cin.tie(0);
    int ttest = 1;
//    cin >> ttest;
    while(ttest--) solve();
    return 0;
}
