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

template<const uint32_t MOD>
class ModInt {
    using ui32 = uint32_t;
    using ui64 = uint64_t;
    using ii32 = int32_t;
    using ii64 = int64_t;
private:
    uint32_t data;

public:
    template<typename Integer>
    ModInt(const Integer a) : data((a % MOD + MOD) % MOD) {
        static_assert(is_integral_v<Integer>);
    }
    ModInt() : data(0) {}
    ModInt(const ModInt<MOD>& other) : data(other.data) {}
    ModInt& operator=(const ModInt<MOD> other) {
        data = other.data;
        return *this;
    }

    template<typename Integer>
    [[maybe_unused]] typename std::enable_if_t<std::is_integral<Integer>::value, Integer>
    static by_modulo(Integer integer) {
        return (integer % MOD + MOD) % MOD;
    }

    explicit operator ii32() const {
        return data;
    }
    explicit operator ui32() const {
        return data;
    }
    explicit operator ii64() const {
        return data;
    }
    explicit operator ui64() const {
        return data;
    }

    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>
    operator+(const Integer integer) const {
        return ModInt(data + integer);
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>&
    operator+=(const Integer integer) {
        this->data = by_modulo(1ll * this->data + integer);
        return *this;
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>&
    operator++() {
        return *this += 1;
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>
    operator++(int) {
        ModInt<MOD> tmp = *this;
        *this += 1;
        return tmp;
    }

    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>
    operator-(const Integer integer) const {
        return ModInt(1ll * data - integer);
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>&
    operator-=(const Integer integer) {
        this->data = by_modulo(1ll * data - integer);
        return *this;
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>&
    operator--() {
        return *this -= 1;
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>
    operator--(int) {
        ModInt<MOD> tmp = *this;
        *this -= 1;
        return tmp;
    }

    ModInt operator+(const ModInt<MOD> other) const {
        return *this + ui32(other);
    }
    ModInt operator+=(const ModInt<MOD> other) {
        return (*this += ui32(other));
    }

    ModInt operator-(const ModInt<MOD> other) const {
        return *this - ui32(other);
    }
    ModInt operator-=(const ModInt<MOD> other) {
        return (*this -= ui32(other));
    }

    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>
    operator*(const Integer integer) const {
        return ModInt(1ll * data * integer);
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>&
    operator*=(const Integer integer) {
        data = by_modulo(1ll * data * integer);
        return *this;
    }

    ModInt operator*(const ModInt& other) const {
        return *this * ui32(other);
    }
    ModInt& operator*=(const ModInt& other) {
        return (*this *= ui32(other));
    }

    template<typename UInteger>
    typename std::enable_if_t<std::is_integral<UInteger>::value, ModInt>
    operator^(UInteger power) const {
        ModInt<MOD> cur = *this, res = 1;
        while (power) {
            if (power & 1) {
                res *= cur;
            }

            cur *= cur;
            power >>= 1;
        }

        return res;
    }

    template<typename UInteger>
    typename std::enable_if_t<std::is_integral<UInteger>::value, ModInt>&
    operator^=(UInteger power) {
        ModInt<MOD> res = 1;
        while (power) {
            if (power & 1) {
                res *= *this;
            }
            *this *= *this;
            power >>= 1;
        }

        return (*this = res);
    }

    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>
    operator/(const Integer integer) const {
        return *this * (ModInt<MOD>(integer) ^ (MOD - 2));
    }
    template<typename Integer>
    typename std::enable_if_t<std::is_integral<Integer>::value, ModInt>&
    operator/=(const Integer integer) {
        return (*this *= (ModInt<MOD>(integer) ^ (MOD - 2)));
    }

    ModInt operator/(const ModInt<MOD> other) const {
        return *this / ui32(other);
    }
    ModInt& operator/=(const ModInt<MOD> other) {
        return (*this /= ui32(other));
    }

    template<typename Ostream> friend Ostream& operator<<(Ostream& ostream, const ModInt arg) {
        return ostream << arg.data;
    }

    template<typename Istream> friend Istream& operator>>(Istream& istream, ModInt& arg) {
        istream >> arg.data;
        arg.data = by_modulo(arg.data);
        return istream;
    }
};

static const int MOD = 998244353;
using mint = ModInt<MOD>;

// tested on https://codeforces.com/contest/2070/problem/D

void solve() {
    int n;
    cin >> n;
    vector<int> parent(n+1, 0);
    for(int i = 2; i <= n; i++){
        cin >> parent[i];
    }
    vector<vector<int>> children(n+1);
    for(int i=2; i<=n; i++){
        int p = parent[i];
        children[p].push_back(i);
    }
    vector<int> dist(n+1, -1);
    dist[1] = 0;
    queue<int>q;
    q.push(1);
    int maxDist = 0;
    while(!q.empty()){
        int v = q.front();
        q.pop();
        int d = dist[v];
        maxDist = max(maxDist, d);
        for(int c: children[v]){
            dist[c] = d+1;
            q.push(c);
        }
    }
    vector<vector<int>> level(maxDist+1);
    for(int v=1; v<=n; v++){
        level[ dist[v] ].push_back(v);
    }

    vector<mint> ways(n+1, 0LL);
    ways[1] = 1;
    for(int d=0; d<maxDist; d++){
        mint sumWays = 0;
        for(int v: level[d]){
            sumWays += ways[v];
        }
        for(int u: level[d+1]){
            ways[u] += sumWays;
        }
        for(int v: level[d]){
            if(v == 1) continue;
            auto wv = ways[v];
            for(int c: children[v]) {
                if(dist[c] == d + 1) {
                    ways[c] -= wv;
                }
            }
        }
    }

    cout << accumulate(all(ways), mint()) << endl;
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
