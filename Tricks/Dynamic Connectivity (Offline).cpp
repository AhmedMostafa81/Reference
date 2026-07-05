#include <iostream>
#include <vector>
#include <numeric>
#include <map>
#include <stack>

using namespace std;

// Disjoint Set Union with Rollback
struct DSU {
    vector<int> parent, sz;
    int components;
    
    struct Action {
        int u, v;
        bool merged;
    };
    stack<Action> history;

    DSU(int n) {
        parent.resize(n + 1);
        sz.resize(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
        components = n;
    }

    // No path compression to maintain O(1) rollback
    int find_set(int v) {
        if (v == parent[v])
            return v;
        return find_set(parent[v]); 
    }

    bool union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a == b) {
            history.push({a, b, false});
            return false;
        }
        // Union by size
        if (sz[a] < sz[b])
            swap(a, b);
            
        parent[b] = a;
        sz[a] += sz[b];
        components--;
        history.push({a, b, true});
        return true;
    }

    void rollback() {
        Action act = history.top();
        history.pop();
        if (act.merged) {
            int a = act.u;
            int b = act.v;
            sz[a] -= sz[b];
            parent[b] = b; // Detach
            components++;
        }
    }
};

// Segment Tree over the Time Domain
struct QueryTree {
    vector<vector<pair<int, int>>> tree;
    int T;

    QueryTree(int T) : T(T) {
        // 4 * T is enough for the segment tree
        tree.resize(4 * T); 
    }

    void add_edge_interval(int node, int l, int r, int ql, int qr, pair<int, int> edge) {
        if (l > qr || r < ql) return;
        if (ql <= l && r <= qr) {
            tree[node].push_back(edge);
            return;
        }
        int mid = l + (r - l) / 2;
        add_edge_interval(2 * node, l, mid, ql, qr, edge);
        add_edge_interval(2 * node + 1, mid + 1, r, ql, qr, edge);
    }

    void dfs(int node, int l, int r, DSU& dsu, const vector<bool>& is_query) {
        // Apply all edges at this segment
        for (const auto& edge : tree[node]) {
            dsu.union_sets(edge.first, edge.second);
        }

        // If we are at a leaf, process the query if one exists at this time step
        if (l == r) {
            if (is_query[l]) {
                cout << dsu.components << "\n";
            }
        } else {
            int mid = l + (r - l) / 2;
            dfs(2 * node, l, mid, dsu, is_query);
            dfs(2 * node + 1, mid + 1, r, dsu, is_query);
        }

        // Rollback state before returning to parent
        for (size_t i = 0; i < tree[node].size(); ++i) {
            dsu.rollback();
        }
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    if (m == 0) return 0;

    map<pair<int, int>, int> active_edges;
    vector<bool> is_query(m, false);
    QueryTree st(m);

    for (int i = 0; i < m; ++i) {
        char type;
        cin >> type;
        if (type == '+') {
            int u, v;
            cin >> u >> v;
            if (u > v) swap(u, v);
            active_edges[{u, v}] = i;
        } else if (type == '-') {
            int u, v;
            cin >> u >> v;
            if (u > v) swap(u, v);
            
            // Add the interval [start_time, i-1] to the segment tree
            st.add_edge_interval(1, 0, m - 1, active_edges[{u, v}], i - 1, {u, v});
            active_edges.erase({u, v});
        } else if (type == '?') {
            is_query[i] = true;
        }
    }

    // Flush any edges that were added but never removed until the end of the queries
    for (const auto& [edge, start_time] : active_edges) {
        st.add_edge_interval(1, 0, m - 1, start_time, m - 1, edge);
    }

    DSU dsu(n);
    st.dfs(1, 0, m - 1, dsu, is_query);

    return 0;
}
