  LCA O(1)





struct LCA_O1 {
    int n;
    vector<vector<int>> adj;
    vector<int> euler;      // Stores the Euler tour of the tree
    vector<int> depth;      // Depth of each node in the Euler tour
    vector<int> first;      // First occurrence of each node in the Euler tour
    vector<vector<int>> st; // Sparse table storing indices
    vector<int> log2_val;   // Precomputed log2 values for O(1) queries

    LCA_O1(int nodes) {
        n = nodes;
        adj.resize(n);
        first.assign(n, -1);
        
        // The Euler tour has length 2N - 1
        log2_val.resize(2 * n);
        log2_val[1] = 0;
        for (int i = 2; i < 2 * n; i++) {
            log2_val[i] = log2_val[i / 2] + 1;
        }
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 1. Traverse the tree to build the Euler tour and record depths
    void dfs(int u, int p = -1, int d = 0) {
        first[u] = euler.size();
        euler.push_back(u);
        depth.push_back(d);
        
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, d + 1);
                // Add the current node again after returning from a child
                euler.push_back(u);
                depth.push_back(d);
            }
        }
    }

    // 2. Build the Sparse Table over the Euler tour
    void build(int root = 0) {
        euler.clear();
        depth.clear();
        dfs(root);
        
        int m = euler.size();
        int K = log2_val[m] + 1;
        st.assign(K, vector<int>(m));

        // Base case: intervals of length 1
        for (int i = 0; i < m; i++) {
            st[0][i] = i; 
        }

        // DP to build the Sparse Table
        for (int j = 1; (1 << j) <= m; j++) {
            for (int i = 0; i + (1 << j) <= m; i++) {
                int idx1 = st[j - 1][i];
                int idx2 = st[j - 1][i + (1 << (j - 1))];
                
                // We want the index in the Euler tour with the minimum depth
                if (depth[idx1] < depth[idx2]) {
                    st[j][i] = idx1;
                } else {
                    st[j][i] = idx2;
                }
            }
        }
    }

    // 3. Query the LCA in O(1) time
    int get_lca(int u, int v) {
        int l = first[u];
        int r = first[v];
        if (l > r) swap(l, r);
        
        int j = log2_val[r - l + 1];
        int idx1 = st[j][l];
        int idx2 = st[j][r - (1 << j) + 1];
        
        if (depth[idx1] < depth[idx2]) {
            return euler[idx1];
        } else {
            return euler[idx2];
        }
    }

    // 4. Get exact distance between two nodes in O(1) time
    int get_dist(int u, int v) {
        int lca = get_lca(u, v);
        return depth[first[u]] + depth[first[v]] - 2 * depth[first[lca]];
    }
};
