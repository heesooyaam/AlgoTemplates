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

template<const int MAXV, const int ALL_DIVS = MAXV * (int) pow(MAXV, 1.0 / 3)>
class divisors_sieve_t {
private:
    array<int, ALL_DIVS> divisors;
    array<pair<int, int>, ALL_DIVS> borders;

    /*
     * to simple access to divisors;
     * for e.g.: for (int d : sieve.get_divisors(x)) { ... }
     */
    struct divisors_t {
        divisors_t(int x, const array<int, ALL_DIVS>& divs, const array<pair<int, int>, ALL_DIVS>& borders)
                : x(x)
                , divs(divs)
                , borders(borders) {}

        explicit divisors_t(const divisors_t& other)
                : x(other.x) {}

        const int x;
        const array<int, ALL_DIVS>& divs;
        const array<pair<int, int>, ALL_DIVS>& borders;
        array<int, ALL_DIVS>::const_iterator begin() {
            return divs.begin() + borders[x].ff;
        }

        array<int, ALL_DIVS>::const_iterator end() {
            return divs.begin() + borders[x].ss;
        }
    };

    void build_sieve() {
        for (int i = 1; i < MAXV + 1; ++i) {
            for (int j = i; j < MAXV + 1; j += i) {
                ++borders[j].ss;
            }
        }

        for (int i = 1, pref = 0; i < MAXV + 1; ++i) {
            borders[i].ff = pref;
            pref += borders[i].ss;
            borders[i].ss = borders[i].ff;
        }

        for (int i = 1; i < MAXV + 1; ++i) {
            for (int j = i; j < MAXV + 1; j += i) {
                divisors[borders[j].ss++] = i;
                while (borders[j].ss > ALL_DIVS) {
                    cout << "UNLUCK";
                    cout.flush();
                }
            }
        }
    }

public:
    divisors_sieve_t() : borders() {
        build_sieve();
    }

    // u should use it to find ALL_DIVS exactly
    static int count_all_divs() {
        int cnt = 0;
        for (int i = 1; i < MAXV + 1; ++i) {
            cnt += MAXV / i;
        }

        return cnt;
    }

    const array<int, ALL_DIVS>& get_all_divisors() const {
        return divisors;
    }

    pair<int, int> get_borders(int x) const {
        return borders[x];
    }

    int get_amount_of_divisors(int x) const {
        return borders[x].ss - borders[x].ff;
    }

    divisors_t get_divisors_of(int x) {
        return divisors_t(x, divisors, borders);
    }
};

const int MAXA = 1'000'000;
const int ALL_DIVS = 14'970'034;

// task: given array arr of size n, find amount of pairs (i, j), i < j,
// such that gcd(arr[i], arr[j]) == XOR(a[i], a[j])
// 1 <= a[i], n <= 1'000'000

void solve() {
    int n;
    cin >> n;
    vector<int> vec(n);
    input(vec);
    map<int, int> cnt;
    ll ans = 0;
    divisors_sieve_t<MAXA, ALL_DIVS> sieve;
    cnt[vec[0]] = 1;
    for (int i = 1; i < n; ++i) {
        for (auto mb_gcd : sieve.get_divisors_of(vec[i])) {
            int to_find = vec[i] ^ mb_gcd;

            if (gcd(vec[i], to_find) != mb_gcd) {
                continue;
            }

            ans += cnt[to_find];
        }

        ++cnt[vec[i]];
    }

    cout << ans << endl;
}

int32_t main() {
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    ios::sync_with_stdio(0); cin.tie(0);
    int ttest = 1;
    cin >> ttest;
    while(ttest--) solve();
    return 0;
}
