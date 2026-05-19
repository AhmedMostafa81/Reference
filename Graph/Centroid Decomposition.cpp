const int N = ;
vector<int>gr[N] , par(N);
int sz[N];
bool del[N];

void init_size(int node , int cur_par) {
    sz[node] = 0 ;
    if (del[node])
        return ;
    sz[node] = 1;
    for (auto ch:gr[node])
        if (ch != cur_par) {
            init_size(ch , node);
            sz[node] += sz[ch];
        }
}

int centroid(int node , int cur_par , int tot) {
    for (auto ch:gr[node])
        if (cur_par != ch)
            if (!del[ch] && sz[ch] > tot / 2)
                return centroid(ch , node , tot);
    return node;
}

void decomposition(int node , int cur_par) {
    init_size(node , -1);
    int x = centroid(node , -1 , sz[node]);
    par[x] = cur_par;
    del[x] = true;
    for (auto ch:gr[x])
        if (!del[ch])
            decomposition(ch , x);
}

void update(int node) {
    int t = node;
    while (~node) {
        // CODE
        node = par[node];
    }
}

int query(int node) {
    int t = node;
    while (~node) {
        // CODE        
        node = par[node];
    }
    return ;
}
--------------------------------------------------------------------
// another version

int MX_DEP;

void solve(int node , int p , bool Fill , int dep) {
    MX_DEP = max(MX_DEP , dep);
    if (Fill)
        fr[dep]++;
    else if (k >= dep)
        ans += fr[k - dep];
    for (auto ch:gr[node])
        if (!del[ch] && ch != p)
            solve(ch , node , Fill , dep + 1);
}

void decomposition(int node) {
    get_sz(node , -1);
    int cen = centroid(node , -1 , sz[node]);
    del[cen] = true;
    MX_DEP = 0 ;
    fr[0] = 1; // add this node
    for (auto ch:gr[cen]) {
        if (!del[ch]) {
            solve(ch , cen , 0 , 1);
            solve(ch , cen , 1 , 1);
        }
    }
    for (int i = 0; i <= MX_DEP ; i++)
        fr[i] = 0;
    for (auto ch:gr[cen])
        if (!del[ch])
            decomposition(ch);
}

    
// ------------------------------------------------

// to count all pahts that pass from each node



//    ->> practice makes perfect


#include<bits/stdc++.h>
using namespace std;
#define hi cerr<<"Hi\n";

/* ===================  NO CLEAN CODE HERE  =================== */

const int N = 2e5+5;
vector<int>gr[N];
string s;
int a[N] , sz[N];
long long  ans[N] ;
bool del[N];

void init_sz(int node , int p) {
    sz[node] = 0 ;
    if (del[node])
        return ;
    sz[node] = 1;
    for (auto ch:gr[node]) {
        if (ch != p) {
            init_sz(ch , node);
            sz[node] += sz[ch];
        }
    }
}

int centroid(int node , int p , int tot) {
    for (auto ch:gr[node])
        if (ch != p && !del[ch] && sz[ch] > tot / 2 )
            return centroid(ch , node , tot);
    return node;
}

int fr[1 << 20];

void dfs(int node , int p , int mask , int val) {
    fr[mask] += val;
    for (auto ch:gr[node])
        if (!del[ch] && ch != p)
            dfs(ch , node , mask ^ a[ch] , val);
}

long long solve(int node , int p , int mask) {
    long long rt = fr[mask];
    for (int i = 0 ; i < 20 ; i++)
        rt += fr[mask ^ (1 << i)];
    for (auto ch:gr[node])
        if (ch != p && !del[ch])
            rt += solve(ch , node , mask ^ a[ch]);
    ans[node] += rt;
    return rt;
}

void decomposition(int node ) {
    init_sz(node , -1);
    int cen = centroid(node , -1 , sz[node]);
    del[cen] = true;
    long long rt = 0 ;
    dfs(cen , -1 , a[cen] , 1);
    rt += fr[0];
    for (int i = 0; i < 20 ; i++)
        rt += fr[1 << i];
    for (auto ch:gr[cen]) {
        if (del[ch])continue;
        dfs(ch , cen , a[cen] ^ a[ch] , -1);
        rt += solve(ch , cen , a[ch]);
        dfs(ch , cen , a[cen] ^ a[ch] , 1);
    }
    dfs(cen , -1 , a[cen] , -1);
    ans[cen] += rt >> 1;
    for (auto ch:gr[cen])
        if (!del[ch])
            decomposition(ch);
}

void maybe() {

    int n ;cin >> n ;
    for (int i  =0 ; i < n - 1; i++) {
        int x,y;cin >> x >> y;
        gr[x].push_back(y);
        gr[y].push_back(x);
    }
    cin >> s;
    for (int i  = 0 ; i < n; i++)
        a[i + 1] = 1 << (s[i] - 'a');
    decomposition(1);
    for (int i =1;i<=n ; i++)
        cout << ans[i] + 1 << ' ';
    cout << '\n';

}


signed main() {

    ios::sync_with_stdio(0);cin.tie(0);
    int tc = 1;
    //cin>>tc;
    while(tc--)
        maybe();

}
