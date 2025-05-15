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

template <typename cont_t, array MOD, array base>
class hasher {
private:
    const int n;
    const int pts;
    vector<array<ll, MOD.size()>> rev;
    vector<array<ll, MOD.size()>> dir;
    inline static vector<array<ll, MOD.size()>> x;

public:
    struct dir_hash_t;
    struct rev_hash_t {
        array<ll, MOD.size()> h;

        bool operator==(const hasher<cont_t, MOD, base>::dir_hash_t &other) const {
            return other == *this;
        }

        bool operator==(const hasher<cont_t, MOD, base>::rev_hash_t &other) const {
            return h == other.h;
        }
    };

    struct dir_hash_t {
        int k; // h[i] = x^k * (a[i] + a[i+1] * x^1 + ...)
        array<ll, MOD.size()> h;

        bool operator==(const hasher<cont_t, MOD, base>::dir_hash_t &other) const {
            if (k == other.k) {
                return h == other.h;
            }

            int d = abs(k - other.k);
            const auto &h1 = (k < other.k ? h : other.h);
            const auto &h2 = (k < other.k ? other.h : h);

            bool eq = true;
            for (int i = 0; i < h.size() && eq; ++i) {
                eq &= (h1[i] * hasher<cont_t, MOD, base>::x[d][i] % MOD[i]) == h2[i];
            }

            return eq;
        }

        bool operator==(const hasher<cont_t, MOD, base>::rev_hash_t &other) const {
            bool eq = true;
            for (int i = 0; i < h.size() && eq; ++i) {
                eq &= (other.h[i] * hasher<cont_t, MOD, base>::x[k][i] % MOD[i]) == h[i];
            }

            return eq;
        }
    };

    friend struct dir_hash_t;

    hasher(const cont_t &data, bool need_both_hashes = false, bool cur_ind = 1)
    : n(data.size() - cur_ind), pts(MOD.size()), rev(n + 1), dir(need_both_hashes * (n + 1)) {
        build_pows();
        build_rev_hash(data, cur_ind);
        if (need_both_hashes) {
            build_dir_hash(data, cur_ind);
        }
    }

    rev_hash_t get_rev_hash(int l, int r) const {
        rev_hash_t h;
        for (int pt = 0; pt < pts; ++pt) {
            h.h[pt] = (rev[r][pt] - rev[l - 1][pt] * x[r - l + 1][pt]) % MOD[pt];
            h.h[pt] = (h.h[pt] < 0 ? h.h[pt] + MOD[pt] : h.h[pt]);
        }

        return h;
    }

    dir_hash_t get_dir_hash(int l, int r) const {
        assert(!dir.empty());
        
        dir_hash_t h{.k = l - 1};
        for (int pt = 0; pt < pts; ++pt) {
            h.h[pt] = dir[r][pt] - dir[l - 1][pt];
            h.h[pt] = (h.h[pt] < 0 ? h.h[pt] + MOD[pt] : h.h[pt]);
        }

        return h;
    }

    bool is_palindrome(int l, int r) {
        assert(!dir.empty());

        return get_dir_hash(l, r) == get_rev_hash(l, r);
    }

private:
    void build_pows() {
        if (x.empty()) {
            x.reserve(n + 1);
            x.resize(2);

            fill(all(x[0]), 1);
            for (int i = 0; i < pts; ++i) {
                x[1][i] = base[i] % MOD[i];
            }
        }

        x.reserve(n + 1);
        for (int pw = x.size(); pw < n + 1; ++pw) {
            x.eb();
            for (int pt = 0; pt < pts; ++pt) {
                x[pw][pt] = x[pw - 1][pt] * x[1][pt] % MOD[pt];
            }
        }
    }

    void build_dir_hash(const cont_t &data, bool cur_ind) {
        assert(!dir.empty());

        for (int i = 1; i < n + 1; ++i) {
            for (int pt = 0; pt < pts; ++pt) {
                dir[i][pt] = (dir[i - 1][pt] + x[i - 1][pt] * data[i - !cur_ind]) % MOD[pt];
            }
        }
    }

    void build_rev_hash(const cont_t &data, bool cur_ind) {
        for (int i = 1; i < n + 1; ++i) {
            for (int pt = 0; pt < pts; ++pt) {
                rev[i][pt] = (rev[i - 1][pt] * x[1][pt] + data[i - !cur_ind]) % MOD[pt];
            }
        }
    }
};
template <typename T> using arr_hasher = hasher<vector<T>, array{(int)1e9 + 7, (int)1e9 + 23}, array{(int)2e5 + 5, (int)3e5 + 23}>;
using string_hasher = hasher<string, array{(int)1e9 + 7, (int)1e9 + 23}, array{(int)237, (int)192}>;

// tested on C from https://codeforces.com/gym/100133

void solve() {
    string s;
    cin >> s;
    s = " " + s;
    string_hasher hash(s);
    int q;
    cin >> q;
    while (q--) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;

        cout << (hash.get_rev_hash(l1, r1) == hash.get_rev_hash(l2, r2) ? "Yes" : "No") << endl;
    }
}

int32_t main() {
//    freopen("substrcmp.in", "r", stdin);
//    freopen("substrcmp.out", "w", stdout);
    ios::sync_with_stdio(0); cin.tie(0);
    int ttest = 1;
//    cin >> ttest;
    while(ttest--) solve();
    return 0;
}
