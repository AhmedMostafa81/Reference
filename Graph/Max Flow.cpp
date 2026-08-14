
/*
    ================================================================================
    MAX FLOW / MIN CUT THEORIES & MODELING CHEAT SHEET
    ================================================================================

    1. Max Flow == Min Cut
       The maximum flow from S to T equals the minimum capacity of edges you
       need to remove to completely disconnect S from T.

    2. Vertex Capacities (Node splitting)
       If a NODE `u` has a capacity `C` (can only be visited C times), split it:
       Create `u_in` and `u_out`. Add edge: (u_in -> u_out) with capacity C.
       All incoming edges go to `u_in`. All outgoing edges leave from `u_out`.

    3. Undirected Edges
       Add directed edge (u -> v, cap) AND (v -> u, cap). (Helper added in Dinic)

    4. Bipartite Matching (Konig's Theorem)
       - Max Bipartite Matching = Max Flow (S -> L, L -> R, R -> T, all caps = 1)
       - Min Vertex Cover (Bipartite) = Max Bipartite Matching
       - Max Independent Set (Bipartite) = Total Vertices - Max Matching

    5. Minimum Path Cover in a DAG (Directed Acyclic Graph)
       Find the minimum number of paths to visit every node exactly once.
       - Answer = V - MaxBipartiteMatching
       - To build: Split every node `u` into `u_out` (Left) and `u_in` (Right).
         For every edge u->v in DAG, add edge `u_out` -> `v_in` with cap 1.
    6. Maximum Weight Closure
        Think of projects as positive nodes and tools/costs as negative nodes.

        A project gives profit.
        A tool has a cost, and multiple projects can use the same tool, so we pay for that tool only once.
        If a project requires a tool, choosing the project forces us to choose that tool.
        Max Flow Construction
        Positive node:   S → node     capacity = profit
        Negative node:   node → T     capacity = cost
        Requirement:     project → tool   capacity = INF

        Interpret:
        S-side = chosen
        T-side = not chosen
        The INF edge guarantees that:
        choose project → must choose required tool
        So the min-cut represents:
        lost profits + costs of chosen tools
        Therefore:
        Answer = sum of all positive profits − MaxFlow

        Important: If several chosen projects need the same tool, that tool is selected once, so its cost is paid once.
==============================================================================
*/


// slow max flow
// O(V * E^2)

struct EdmondsKarp {
    int n;
    vector<vector<long long>> capacity;
    vector<vector<int>> adj;

    EdmondsKarp(int n) : n(n), capacity(n, vector<long long>(n, 0)), adj(n) {}

    void add_edge(int u, int v, long long cap) {
        if (capacity[u][v] == 0 && capacity[v][u] == 0) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        capacity[u][v] += cap;
    }

    long long bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, long long>> q;
        q.push({s, 1e18});

        while (!q.empty()) {
            int cur = q.front().first;
            long long flow = q.front().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next] > 0) {
                    parent[next] = cur;
                    long long new_flow = min(flow, capacity[cur][next]);
                    if (next == t) return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

    long long maxflow(int s, int t) {
        long long flow = 0, new_flow;
        vector<int> parent(n);

        while ((new_flow = bfs(s, t, parent))) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }
        }
        return flow;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//    fast max flow

struct FlowEdge { // O(E * sqrt(V))
    int v, u;
    long long cap, flow = 0;
    FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
    const long long flow_inf = 1e18;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, m = 0;
    int s, t;
    vector<int> level, ptr;
    queue<int> q;

    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, long long cap) {
        edges.emplace_back(v, u, cap);
        edges.emplace_back(u, v, 0);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }

    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap == edges[id].flow)
                    continue;
                if (level[edges[id].u] != -1)
                    continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    long long dfs(int v, long long pushed) {
        if (pushed == 0)
            return 0;
        if (v == t)
            return pushed;
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u])
                continue;
            long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long flow() {
        long long f = 0;
        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs())
                break;
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, flow_inf)) {
                f += pushed;
            }
        }
        return f;
    }

    vector<vector<int>> extract_flow_paths(long long flow_limit = -1) {
    vector<vector<int>> paths;

    while (true) {
        vector<int> path = {s};
        vector<bool> visited(n, false);
        long long pushed = flow_inf;

        bool found = false;
        function<bool(int)> dfs = [&](int v) -> bool {
            if (v == t) return true;
            visited[v] = true;
            for (int& i = ptr[v]; i < (int)adj[v].size(); ++i) {
                int id = adj[v][i];
                FlowEdge& e = edges[id];
                if (e.flow > 0 && !visited[e.u]) {
                    path.push_back(e.u);
                    long long minflow = min(pushed, e.flow);
                    pushed = minflow;
                    if (dfs(e.u)) {
                        e.flow -= pushed;
                        edges[id ^ 1].flow += pushed;
                        return true;
                    }
                    path.pop_back();
                }
            }
            return false;
        };

        fill(ptr.begin(), ptr.end(), 0);
        if (!dfs(s)) break;

        if (flow_limit != -1 && pushed > flow_limit) pushed = flow_limit;
        paths.push_back(path);
        if (flow_limit != -1) {
            flow_limit -= pushed;
            if (flow_limit <= 0) break;
        }
    }

    return paths;
    }


    vector<tuple<int, int, long long>> get_used_edges() {
        vector<tuple<int, int, long long>> result;
        for (const auto& e : edges) {
            if (e.cap > 0 && e.flow > 0) {
                result.emplace_back(e.v, e.u, e.flow);
            }
        }
        return result;
    }

    vector<pair<int, int>> min_cut() {
        // Step 1: Find reachable nodes from s in residual graph
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(s);
        vis[s] = true;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int id : adj[v]) {
                int u = edges[id].u;
                if (!vis[u] && edges[id].flow < edges[id].cap) {
                    vis[u] = true;
                    q.push(u);
                }
            }
        }

        // Step 2: Find saturated forward edges from reachable to unreachable
        vector<pair<int, int>> cut_edges;
        for (int i = 0; i < (int)edges.size(); i += 2) {
            const FlowEdge& e = edges[i];
            if (e.cap == e.flow && e.cap > 0 && vis[e.v] && !vis[e.u]) {
                cut_edges.emplace_back(e.v, e.u);
            }
        }

        return cut_edges;
    }

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    minimum cost max flow

#include <bits/stdc++.h>
using namespace std;

struct FlowEdge {
    int v, u;
    long long cap, flow = 0, cost;

    FlowEdge(int v, int u, long long cap, long long cost)
        : v(v), u(u), cap(cap), cost(cost) {}
};

struct FlowNetwork {
    const long long INF = 1e18;

    int n, m = 0, s, t;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    vector<int> level, ptr;
    queue<int> q;

    FlowNetwork(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, long long cap, long long cost = 0) {
        edges.emplace_back(v, u, cap, cost);
        edges.emplace_back(u, v, 0, -cost);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }

    // Dinic BFS
    bool bfs() {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        q = queue<int>();
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap - edges[id].flow < 1) continue;
                if (level[edges[id].u] != -1) continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    // Dinic DFS
    long long dfs(int v, long long pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;

        for (int &cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;

            if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                continue;

            long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0) continue;

            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    long long max_flow() {
        long long f = 0;
        while (true) {
            if (!bfs()) break;
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, INF)) {
                f += pushed;
            }
        }
        return f;
    }

    // Min Cost Flow of K units using SPFA
    long long min_cost_flow(long long K) {
        long long flow = 0, cost = 0;
        vector<long long> dist(n);
        vector<int> in_queue(n), parent(n), parent_edge(n);

        while (flow < K) {
            fill(dist.begin(), dist.end(), INF);
            fill(in_queue.begin(), in_queue.end(), 0);
            fill(parent.begin(), parent.end(), -1);
            fill(parent_edge.begin(), parent_edge.end(), -1);

            dist[s] = 0;
            queue<int> q;
            q.push(s);
            in_queue[s] = 1;

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                in_queue[v] = 0;

                for (int id : adj[v]) {
                    int u = edges[id].u;
                    if (edges[id].cap - edges[id].flow < 1) continue;

                    if (dist[u] > dist[v] + edges[id].cost) {
                        dist[u] = dist[v] + edges[id].cost;
                        parent[u] = v;
                        parent_edge[u] = id;
                        if (!in_queue[u]) {
                            in_queue[u] = 1;
                            q.push(u);
                        }
                    }
                }
            }

            if (dist[t] == INF) break;

            long long push = K - flow;
            int v = t;
            while (v != s) {
                int id = parent_edge[v];
                push = min(push, edges[id].cap - edges[id].flow);
                v = parent[v];
            }

            flow += push;
            cost += push * dist[t];

            v = t;
            while (v != s) {
                int id = parent_edge[v];
                edges[id].flow += push;
                edges[id ^ 1].flow -= push;
                v = parent[v];
            }
        }

        return (flow < K ? -1 : cost);
    }

    // Returns all original forward edges that carry positive flow.
    // Each tuple = {from, to, used_flow, cost}
    vector<tuple<int, int, long long, long long>> get_used_edges() const {
        vector<tuple<int, int, long long, long long>> res;
        for (int i = 0; i < m; i += 2) {
            const FlowEdge &e = edges[i];   // only forward/original edges
            if (e.cap > 0 && e.flow > 0) {
                res.emplace_back(e.v, e.u, e.flow, e.cost);
            }
        }
        return res;
    }
};
