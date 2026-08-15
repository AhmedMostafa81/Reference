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

/* 
 * Centroid Decomposition (Stronger Version)
 * - Precomputes tree distances in O(1) using a centroid level array.
 * - Prevents overcounting via inclusion-exclusion with two data structures (DS & DS_par).
 * - Total Time: O(N log N) building, O(log N) per update/query.
 * - Total Space: O(N log N) using dynamically sized Fenwick trees.
 */

#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
const int LOG = 20; // Max depth of centroid tree for N = 10^5 is ~17

vector<int> gr[N];
int par[N], sz[N], level[N];
bool del[N];
int cd_dist[LOG][N]; // cd_dist[lvl][u] = distance from centroid at 'lvl' to node 'u'

// Dynamically sized Fenwick tree to save memory O(N log N) total
struct BIT {
    vector<int> tree;
    void init(int n) { tree.assign(n + 2, 0); }
    void add(int i, int val) {
        for (i++; i < tree.size(); i += i & -i) tree[i] += val;
    }
    int query(int i) {
        int sum = 0;
        for (i = min((int)tree.size() - 1, i + 1); i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

BIT DS[N], DS_par[N];

// O(1) distance query using precomputed level distances
int get_dist(int node, int ancestor) {
    return cd_dist[level[ancestor]][node];
}

void init_size(int node, int cur_par) {
    sz[node] = 1;
    for (auto ch : gr[node]) {
        if (ch != cur_par && !del[ch]) {
            init_size(ch, node);
            sz[node] += sz[ch];
        }
    }
}

int centroid(int node, int cur_par, int tot) {
    for (auto ch : gr[node]) {
        if (ch != cur_par && !del[ch] && sz[ch] > tot / 2) {
            return centroid(ch, node, tot);
        }
    }
    return node;
}

// Precompute distances from the current centroid to all nodes in its component
void get_dists(int u, int p, int d, int lvl) {
    cd_dist[lvl][u] = d;
    for (auto v : gr[u]) {
        if (v != p && !del[v]) {
            get_dists(v, u, d + 1, lvl);
        }
    }
}

void decomposition(int node, int cur_par, int lvl = 0) {
    init_size(node, -1);
    int x = centroid(node, -1, sz[node]);
    
    par[x] = cur_par;
    level[x] = lvl; // Store the level of this centroid
    del[x] = true;
    
    // --- PRECOMPUTE DISTANCES ---
    get_dists(x, -1, 0, lvl);
    
    // Initialize the data structures for this centroid based on component size.
    // The max distance to any node in this component is bounded by sz[node].
    DS[x].init(sz[node]);
    if (cur_par != -1) {
        // DS_par[x] stores distances from nodes in x's subtree to x's PARENT.
        // Bounded by sz[node] + 1.
        DS_par[x].init(sz[node] + 1); 
    }

    for (auto ch : gr[x]) {
        if (!del[ch]) {
            decomposition(ch, x, lvl + 1);
        }
    }
}

void update(int node, int val) {
    int cur = node;
    
    // Update the centroid itself
    DS[cur].add(0, val);
    
    while (par[cur] != -1) {
        int p = par[cur];
        int dist_to_p = get_dist(node, p);
        
        // 1. Add to parent's DS
        DS[p].add(dist_to_p, val);
        
        // 2. Add to the exclusion DS so we can subtract it later
        DS_par[cur].add(dist_to_p, val);
        
        cur = p;
    }
}

int query(int node, int k) {
    int cur = node;
    
    // Base answer is querying the node's own centroid-subtree
    int ans = DS[cur].query(k);
    
    while (par[cur] != -1) {
        int p = par[cur];
        int dist_to_p = get_dist(node, p);
        
        if (dist_to_p <= k) {
            int remaining_dist = k - dist_to_p;
            
            // Add everything valid from the parent's subtree
            ans += DS[p].query(remaining_dist);
            
            // Subtract the overcounted branch we just came from!
            ans -= DS_par[cur].query(remaining_dist);
        }
        cur = p;
    }
    return ans;
}
    
--------------------------------------------------------------------
// another version

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
