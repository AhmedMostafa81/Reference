// CP-Algo


struct _2_SAT {
    int m;
    int n;
    vector<vector<int>> gr, gr_;
    vector<bool> vis;
    vector<int> order, comp;
    vector<bool> rt;

    _2_SAT (int _n_vars) : m(_n_vars), n(2 * m), gr(n), gr_(n), vis(n), order(), comp(n, -1), rt(m) {
        order.reserve(n);
    }
    void dfs1(int node) {
        vis[node] = true;
        for (int ch : gr[node]) {
            if (!vis[ch])
                dfs1(ch);
        }
        order.push_back(node);
    }

    void dfs2(int node, int cl) {
        comp[node] = cl;
        for (int ch : gr_[node]) {
            if (comp[ch] == -1)
                dfs2(ch, cl);
        }
    }

    bool solve_2SAT() {
        order.clear();
        vis.assign(n, false);
        for (int i = 0; i < n; ++i) {
            if (!vis[i])
                dfs1(i);
        }

        comp.assign(n, -1);
        for (int i = 0, j = 0; i < n; ++i) {
            int node = order[n - i - 1];
            if (comp[node] == -1)
                dfs2(node, j++);
        }

        rt.assign(m, false);
        for (int i = 0; i < n; i += 2) {
            if (comp[i] == comp[i + 1])
                return false;
            rt[i / 2] = comp[i] > comp[i + 1];
        }
        return true;
    }

    void add_disjunction(int a, bool na, int b, bool nb) {
        // na and nb signify whether a and b are to be negated
        a = 2 * a ^ na;
        b = 2 * b ^ nb;
        int neg_a = a ^ 1;
        int neg_b = b ^ 1;
        gr[neg_a].push_back(b);
        gr[neg_b].push_back(a);
        gr_[b].push_back(neg_a);
        gr_[a].push_back(neg_b);
    }

    static void example_usage() {
        _2_SAT solver(3); // a, b, c
        solver.add_disjunction(0, false, 1, true);  //     a  v  not b
        solver.add_disjunction(0, true, 1, true);   // not a  v  not b
        solver.add_disjunction(1, false, 2, false); //     b  v      c
        solver.add_disjunction(0, false, 0, false); //     a  v      a
        assert(solver.solve_2SAT() == true);
        auto expected = vector<bool>(True, False, True);
        assert(solver.rt == expected);
    }
};

// Bedoo

int n, m, r[N];

vector<int> adj[2 * N], rev[2 * N], sw[2 * N];

vector<bool> used;
vector<int> order, comp;
vector<bool> assignment;

void dfs1(int v) {
    used[v] = true;

    for (int u : adj[v]) {
        if (!used[u])
            dfs1(u);
    }

    order.push_back(v);
}

void dfs2(int v, int cl) {
    comp[v] = cl;

    for (int u : rev[v]) {
        if (comp[u] == -1)
            dfs2(u, cl);
    }
}

bool solve_2SAT(int n) {
    order.clear();

    used.assign(n, false);

    for (int i = 0; i < n; ++i) {
        if (!used[i])
            dfs1(i);
    }

    comp.assign(n, -1);

    for (int i = 0, j = 0; i < n; ++i) {
        int v = order[n - i - 1];

        if (comp[v] == -1)
            dfs2(v, j++);
    }

    assignment.assign(n / 2, false);

    for (int i = 0; i < n; i += 2) {
        if (comp[i] == comp[i + 1])
            return false;

        assignment[i / 2] = comp[i] > comp[i + 1];
    }

    return true;
}

int V(int a) {
    return (a << 1);
}

int Ne(int a) {
    return (a << 1) | 1;
}

void edge(int a, int b) {
    adj[a].push_back(b);
    rev[b].push_back(a);
}

void OR(int a, int b) {
    edge(a ^ 1, b);
    edge(b ^ 1, a);
}

void XNOR(int a, int b) {
    OR(Ne(a), V(b));
    OR(V(a), Ne(b));
}

void XOR(int a, int b) {
    OR(Ne(a), Ne(b));
    OR(V(a), V(b));
}

int main() {
    cin >> n >> m;

    for (int i = 1; i <= n; i++)
        cin >> r[i];

    for (int i = 1; i <= m; i++) {
        int k;
        cin >> k;

        for (int j = 1; j <= k; j++) {
            int x;
            cin >> x;

            sw[x].push_back(i);
        }
    }

    for (int i = 1; i <= n; i++) {
        int p = sw[i][0] - 1;
        int q = sw[i][1] - 1; // zero based

        if (!r[i]) {
            XOR(p, q);
        } else {
            XNOR(p, q);
        }
    }

    bool ans = solve_2SAT(2 * m);

    if (ans)
        cout << "YES\n";
    else
        cout << "NO\n";

    return 0;
}

