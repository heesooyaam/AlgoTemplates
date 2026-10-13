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

class traverser;
class suffix_automaton {
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
        ui occ = 1; // not improtant

    private:
        array<ui, 26> go{};
    };

private:
    template<typename ...T>
    ui create(T&&... x) {
        st.emplace_back(std::forward<T>(x)...);
        return st.size() - 1;
    }

    void push_back(char c) {
        const ui newstate = create();
        get(newstate).len = len + 1;

        ui v = last;
        while (v && !get(v).get_next(c)) {
            get(v).set_next(c, newstate);
            v = get(v).sufflink;
        }

        if (!v) {
            get(newstate).sufflink = 1;
        } else {
            const ui q = get(v).get_next(c);
            if (get(v).len + 1 == get(q).len) {
                get(newstate).sufflink = q;
            } else {
                const ui qclone = create(get(q));
                get(qclone).len = get(v).len + 1;
                get(qclone).occ = 0;
                get(q).sufflink = qclone;
                get(newstate).sufflink = qclone;

                while (v && get(v).get_next(c) == q) {
                    get(v).set_next(c, qclone);
                    v = get(v).sufflink;
                }
            }
        }

        last = newstate;
        ++len;
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

public:
    suffix_automaton()
        : last(1)
    {
        create(); // to make root != 0
        create(); // root
    }

    suffix_automaton(string_view s)
        : suffix_automaton()
    {
        st.reserve(2 * s.size() + 2);
        add(s);
    }

    void add(string_view s) {
        for (auto c : s) {
            push_back(c);
        }
    }

    // not important
    void count_occurrences() {
        vector<ui> order(st.size() - 2);
        iota(all(order), 2);
        sort(all(order), [&](ui a, ui b) {
            return get(a).len > get(b).len;
        });

        for (ui v : order) {
            sufflink(v).occ += get(v).occ;
        }

        get(1).occ = 0;
    }

    // not important
    pair<ui, ui> get_state_size(ui v) {
        if (v == root) {
            return {0, 0};
        }
        return {sufflink(v).len + 1, get(v).len};
    }

private:
    friend class traverser;
    static constexpr ui root = 1;
    ui len = 0;
    ui last = 0;
    vector<node> st;
};

class traverser {
public:
    using ui = uint32_t;
    using node = suffix_automaton::node;

    explicit traverser(suffix_automaton& sa, ui start_state = suffix_automaton::root)
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
        cur = suffix_automaton::root;
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

    // not important
    ui get_length() const {
        return len;
    }

private:
    ui cur;
    ui len = 0; // not important
    suffix_automaton& sa;
};

// sample of how to use sa + traversal
int lcs(suffix_automaton& sa, string_view t) {
    int i = 0;
    int ans = 0;
    traverser tr(sa);
    while (i < t.size()) {
        while (!tr.can_move_by_char(t[i]) && tr.can_move_by_sufflink()) {
            tr.move_by_sufflink();
        }

        if (tr.can_move_by_char(t[i])) {
            tr.move_by_char(t[i]);
            ans = max(ans, static_cast<int>(tr.get_length()));
        }
        ++i;
    }
    return ans;
}

// solution of https://codeforces.com/contest/236/problem/E
void solve() {
    string s;
    cin >> s;
    int q;
    suffix_automaton sa(s);
    traverser tr(sa);
    cin >> q;
    while (q--) {
        string x;
        cin >> x;
        const int n = x.size();
        x.reserve(x.size() + x.size() - 1);
        for (int i = 0, bord = 2 * n - 1; i < bord; ++i) {
            x.push_back(x[i]);
        }
        tr.move_to_root();
        int i = 0;
        ll ans = 0;
        static unordered_set<int> st;
        st.clear();
        while (i < x.size()) {
            while (!tr.can_move_by_char(x[i]) && tr.can_move_by_sufflink()) {
                tr.move_by_sufflink();
            }

            if (tr.can_move_by_char(x[i])) {
                tr.move_by_char(x[i]);
                if (tr.get_length() > n) {
                    tr.move_to_suffix(n);
                }
                if (tr.get_length() == n) {
                    st.emplace(tr.get_current_state_id());
                }
            }

            ++i;
        }
        for (auto stid : st) {
            ans += sa.get(stid).occ;
        }
        cout << ans << endl;
    }
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
