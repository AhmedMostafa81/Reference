vector<int> get_centers() {
    vector<int> deg(n + 5, 0);
    for (int i = 1; i <= n; ++i) deg[i] = adj[i].size();

    queue<int> leafs;
    for (int i = 1; i <= n; ++i) if (deg[i]<=1) leafs.push(i);
    int rem = n;
    while (rem > 2) {
        int sz = leafs.size();
        rem -= sz;
        for (int i = 0; i < sz; ++i) {
            int u = leafs.front();
            leafs.pop();
            for (auto&v : adj[u])
                if (--deg[v] == 1) leafs.push(v);
        }
    }
    vector<int> cent;
    while (!leafs.empty()) {
        cent.push_back(leafs.front());
        leafs.pop();
    }
    return cent;
}
