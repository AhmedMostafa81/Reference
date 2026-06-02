/*
all the edges should be in the same connected component
#undirected graph: euler path: all degrees are even or exactly two of them are odd.
#undirected graph: euler circuit: all degrees are even
*/
 
//euler path in an undirected graph
//it also finds circuit if it exists

const int N = ;

vector<pair<int, int>> g[N];
vector<int> ans;
int n , m , done[N];
int vis[N * 10]; //number of edges
void dfs(int u) {
    while (done[u] < g[u].size()) {
        auto e = g[u][done[u]++];
        if (vis[e.second]) continue;
        vis[e.second] = 1;
        dfs(e.first);
    }
    ans.push_back(u);
}
 
int solve(int n) {
    int edges = 0;
    ans.clear();
    memset(done, 0, sizeof done);
    memset(vis, 0, sizeof vis); //be careful, this should have the size of edges, not the nodes
    vector<int> deg(n + 1, 0);
    for (int u = 1; u <= n; u++) {
        for (auto e : g[u]) {
            deg[u]++, edges++; //sa7 5las
        }
    }
    int odd = 0, root = 0;
    for (int i = 1; i <= n; i++) {
        if (deg[i] & 1) odd++, root = i;
    }
    if (odd > 2) return 0; //if we want euler circuit, then odd must be = 0
    if (root == 0) {
        for (int i = 1; i <= n; i++) if (deg[i]) root = i;
    }
    if (root == 0) return 1; //empty graph
    dfs(root);
    if (ans.size() != edges / 2 + 1) return 0;
    reverse(ans.begin(), ans.end());
    return 1;
}
 
int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
 
    cin >> n >> m;
    for(int i=1; i<=m; i++)
    {
        int x , y;
        cin >> x >> y;
        g[x].push_back({y,i});
        g[y].push_back({x,i});
    }
 
    bool a = solve(n);
    cout << a << "\n";
    for(auto it : ans)
        cout << it << ' ';
 
    return 0;
}

==========================================================================



  /*
all the edges should be in the same connected component
#directed graph: euler path: for all -> indeg = outdeg, or nodes having (indeg > outdeg) = nodes having 
(outdeg > indeg) = 1 node and the diff between this inDeg and outDeg is 1, and for others inDeg = outDeg
#directed graph: euler circuit: for all -> indeg = outdeg
*/
 
//euler path in a directed graph
//it also finds circuit if it exists

const int N = ;
  
int n , m;
vector<int> g[N], ans;
int done[N];
void dfs(int u) {
    while (done[u] < g[u].size()) dfs(g[u][done[u]++]);
    ans.push_back(u);
}
int solve(int n) {
    int edges = 0;
    vector<int> in(n + 1, 0), out(n + 1, 0);
    for (int u = 1; u <= n; u++) {
        for (auto v : g[u]) in[v]++, out[u]++, edges++;
    }
    int ok = 1, cnt1 = 0, cnt2 = 0, root = 0;
    for (int i = 1; i <= n; i++) {
        if (in[i] - out[i] == 1) cnt1++;
        if (out[i] - in[i] == 1) cnt2++, root = i;
        if (abs(in[i] - out[i]) > 1) ok = 0;
    }
    if (cnt1 > 1 || cnt2 > 1) ok = 0;
    if (!ok) return 0;
    if (root == 0) {
        for (int i = 1; i <= n; i++) if (out[i]) root = i;
    }
    if (root == 0) return 1; //empty graph
    dfs(root);
    if (ans.size() != edges + 1) return 0; //connectivity
    reverse(ans.begin(), ans.end());
    return 1;
}
 
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
 
    cin >> n >> m;
    for(int i=1; i<=m; i++)
    {
        int x,y;
        cin >> x >> y;
        g[x].push_back(y);
    }
    bool a = solve(n);
    cout << a << "\n";
    for(auto it : ans)
        cout << it << ' ';
}
 

