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

template<const int MAXV, const int ALL_PRIMES =  (int) (1.2 * MAXV / log(MAXV))>
class factorization_sieve_t {
private:
    array<int, ALL_PRIMES> primes;
    array<int, MAXV + 1> min_prime_divisor;

    void build_sieve() {
        int last_prime = -1;
        min_prime_divisor[1] = 1;
        for (int i = 2; i < MAXV + 1; ++i) {
            if (!min_prime_divisor[i]) {
                assert(++last_prime < ALL_PRIMES);
                primes[last_prime] = min_prime_divisor[i] = i;
            }

            // k = min_p_div[k] * i => for every i let's look at primes, which are less than min_p_div[i].
            // this way we will get such numbers x, that x = prime * i, where prime <= min_p_div[i], so min_p_div[x] = prime
            // Genius!
            for (int pos = 0; pos <= last_prime && primes[pos] <= min_prime_divisor[i] && i * primes[pos] < MAXV + 1; ++pos) {
                min_prime_divisor[i * primes[pos]] = primes[pos];
            }
        }

        if (last_prime + 1 < ALL_PRIMES) {
            cerr << "Be careful, found primes less than ALL_PRIMES, so u are able to optimize code!" << endl
                 << "found primes: " << last_prime + 1 << ", ALL_PRIMES = " << ALL_PRIMES << endl;
        }
    }

public:

    /*u should use it to find ALL_PRIMES
     * for e.g.:
     *  1st execution:
     *      cout << factorization_sieve_t<MAXV>::count_primes_amount(); (copy printed value)
     *  2nd execution:
     *      const int ALL_PRIMES = *copied value*;
     *      factorization_sieve_t<MAXV, ALL_PRIMES> sieve;
     */
    static int count_primes_amount() {
        int primes_amount = 0;
        array<bool, MAXV + 1> is_prime;
        fill(all(is_prime), true);
        for (int i = 2; i < MAXV + 1; ++i) {
            if (!is_prime[i]) {
                continue;
            }

            ++primes_amount;
            for (int j = 2 * i; j < MAXV + 1; j += i) {
                is_prime[j] = false;
            }
        }

        return primes_amount;
    }

    factorization_sieve_t() : min_prime_divisor() {
        build_sieve();
    }

    const array<int, MAXV + 1>& get_min_prime_divisors() const {
        return min_prime_divisor;
    }

    int get_min_prime_divisor(int x) const {
        return min_prime_divisor[x];
    }

    // returns vector of { {a prime number, power of the prime number} } in inc order
    vector<pair<int, int>> get_factorization_as_pairs(int x) {
        vector<pair<int, int>> factorization;
        while (x != 1) {
            const int mpd = get_min_prime_divisor(x);
            if (factorization.empty() || factorization.back().first == mpd) {
                ++factorization.back().second;
            } else {
                factorization.eb(mpd, 1);
            }

            x /= mpd;
        }

        return factorization;
    }

    // returns dict of { {key: a prime number, value: power of the prime number} }
    template<typename T = map<int, int>>
    T get_factorization_as_dict(int x) {
        return map<int, int>(get_factorization_as_pairs(x));
    }

    const array<int, ALL_PRIMES>& get_all_primes() const {
        return primes;
    }

    bool is_prime(int x) const {
        return x != 1 && min_prime_divisor[x] == x;
    }
};

const int MAXN = 10'000'000;
factorization_sieve_t<MAXN> sieve;

// tested on: https://codeforces.com/problemset/problem/1823/C

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    input(a);

    map<int, int> cnt_divs;
    for (auto x : a) {
        for (auto [d, cnt_d] : sieve.get_factorization_as_pairs(x)) {
            cnt_divs[d] += cnt_d;
        }
    }

    ll ans = 0;
    ll free = 0;
    for (auto& [d, amt] : cnt_divs) {
        ans += amt / 2;
        free += amt & 1;
    }
    ans += free / 3;
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
