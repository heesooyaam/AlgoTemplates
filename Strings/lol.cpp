#include <algorithm>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define int long long
#define all(v) (v).begin(), (v).end()
#define Endl endl

const int mod = 998244353;

int f(int x,int st=mod-2){
    int ans = 1;
    while(st){  
        
        if (st&1){
            ans*=x;ans%=mod;
        }
        st/=2;x*=x;x%=mod;
    }
    return ans;
}

constexpr int inf = 1e18;

struct node {
    int sufflink = 0;
    int len = 0;
    array<int, 26> next{};
    int get_next(char c) {
        return next[c - 'a'];
    }
    void set_next(char c, int x) {
        next[c - 'a'] = x;
    }

};

struct a {
    a() {
        st.emplace_back();
        st.emplace_back();
    }
    int create() {
        st.emplace_back();
        return st.size() - 1;
    }

    void push_back(char c) {
        int newnode = create();
        st[newnode].len = st[last].len + 1;
        int v = last;
        while (v && !st[v].get_next(c)) {
            st[v].set_next(c, newnode);
            v = st[v].sufflink;
        }
        if (!v) {
            st[newnode].sufflink = root;
        } else {
            int q = st[v].get_next(c);
            if (st[v].len + 1 == st[q].len) {
                st[newnode].sufflink = q;
            } else {
                int qclone = create();
                st[qclone] = st[q];
                st[qclone].len = st[v].len + 1;
                st[q].sufflink = qclone;
                st[newnode].sufflink = qclone;
                while (v && st[v].get_next(c) == q) {
                    st[v].set_next(c, qclone);
                    v = st[v].sufflink;
                }
            }
        }
        last = newnode;
    }
    void add( const string& s) {
        for (auto c : s) {
            push_back(c);
        }
    }
    int last = 0;
    int root = 1;
    vector<node> st;
};
a sa;
string f() {
    string ans;
    int v = sa.root;
    while (true) {
        char found = '0';
        for (char c = 'z'; c >= 'a'; --c) {
            if (sa.st[v].get_next(c)) {
                v = sa.st[v].get_next(c);
                found = 'c';
                ans.push_back(c);
                break;
            }
        }
        if (found == '0') {
            return ans;
        }
    }
}
const int NNN = 2e6+1e5;
int sz[NNN+1];
// const int mod = 998244353;
int g(string& S, int ww){
    if (S.empty())return 0;
    int n=S.size();
    char x = S.back();S.pop_back();
    return (g(S,ww-1) + ((int)(x-'a') * sz[ww]))%mod;
}
void solve(){
    int n;
    cin >> n;
    string s;
    cin >> s;
    sa.add(s);
    string mx = f();
    set<char> W;for(auto x:s)W.insert(x);
    char mn1 = *W.begin();
    
    string mn(1, mn1);
    reverse(all(mx));
    int i = g(mn, n-1),j=g(mx,n-1);
    // cout<<"!!!"<<Endl;
    cout<<2*(j-i)<<endl;
    // cout<<"!!!"<<Endl;

}



signed main()  {
    int cur=1;
    for(int i=0;i<=NNN;i++){
        if(i>0)sz[i]=sz[i-1];
        sz[i]+=cur;sz[i]%=mod;
        cur*=26;cur%=mod;
    }
    int t=1;
    cin>>t;
    for(int i=0;i<t;i++){
            solve();
        }return 0;
}
