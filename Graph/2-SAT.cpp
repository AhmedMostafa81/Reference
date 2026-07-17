// Cheat sheet

/*
========================================================================================
                             2-SAT CONSTRAINTS CHEAT SHEET (FULL VERSION)
========================================================================================
Use this table to translate problem statements directly into template function calls.
Note: 'false' means the variable is normal (A), 'true' means it is negated (!A).

--- 1. FORCING VARIABLES (1-Variable) ---
| Problem Constraint                               | Logic      | Function Call                |
|--------------------------------------------------|------------|------------------------------|
| Force A to be permanently True                   | A          | force(A, false)              |
| Force A to be permanently False                  | !A         | force(A, true)               |

--- 2. AT LEAST / AT MOST (OR logic) ---
| Problem Constraint                               | Logic      | Function Call                |
|--------------------------------------------------|------------|------------------------------|
| Must pick at least one of A or B                 | A or B     | add_or(A, false, B, false)   |
| Cannot pick both A and B (At most one)           | !A or !B   | add_or(A, true, B, true)     |
| A and B cannot both be False                     | A or B     | add_or(A, false, B, false)   |
| Either A is True, or B is False                  | A or !B    | add_or(A, false, B, true)    |

--- 3. EXACTLY / EQUIVALENCE (XOR / XNOR logic) ---
| Problem Constraint                               | Logic      | Function Call                |
|--------------------------------------------------|------------|------------------------------|
| Exactly one of A or B must be True               | A ^ B      | add_xor(A, false, B, false)  |
| A and B must be completely opposite              | A != B     | add_xor(A, false, B, false)  |
| Must choose identical options (Both or Neither)  | A == B     | add_xnor(A, false, B, false) |
| If one is False, the other must be False         | A == B     | add_xnor(A, false, B, false) |

--- 4. IMPLICATIONS (IF-THEN logic) ---
| Problem Constraint                               | Logic      | Function Call                |
|--------------------------------------------------|------------|------------------------------|
| If you pick A, you MUST pick B                   | A => B     | add_imp(A, false, B, false)  |
| If you pick A, you CANNOT pick B                 | A => !B    | add_imp(A, false, B, true)   |
| If you DON'T pick A, you MUST pick B             | !A => B    | add_imp(A, true, B, false)   |
| If you DON'T pick A, you CANNOT pick B           | !A => !B   | add_imp(A, true, B, true)    |

--- 5. COMPOUND CONSTRAINTS (Multi-step) ---
| Problem Constraint                               | How to code it in the template        |
|--------------------------------------------------|---------------------------------------|
| A and B MUST BOTH be True                        | force(A, false); force(B, false);     |
| A and B MUST BOTH be False                       | force(A, true); force(B, true);       |
| If A is True, then BOTH B and C must be True     | add_imp(A, false, B, false);          |
|                                                  | add_imp(A, false, C, false);          |
| If A is True, then BOTH B and C must be False    | add_imp(A, false, B, true);           |
|                                                  | add_imp(A, false, C, true);           |
========================================================================================
*/






// CP-Algo

// Variables are 0-indexed. 'na' means "Is Negated A?".
// Example: add_or(x, false, y, true) creates clause: (x OR !y)

struct TwoSAT {
    int n;
    vector<vector<int>> adj, radj;
    vector<int> order, comp;
    vector<bool> vis, ans;

    TwoSAT(int _n) : n(_n) {
        adj.assign(2 * n, vector<int>());
        radj.assign(2 * n, vector<int>());
        ans.assign(n, false);
    }

    // Helper: converts (variable index, is_negated) -> graph node index
    int get_id(int x, bool is_neg) { return (x << 1) | is_neg; }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    // Core primitive: a => b
    void add_imp(int a, bool na, int b, bool nb) {
        int u = get_id(a, na), v = get_id(b, nb);
        add_edge(u, v);
        add_edge(v ^ 1, u ^ 1);
    }

    // Logical gate wrappers
    void add_or(int a, bool na, int b, bool nb) { add_imp(a, !na, b, nb); }
    void add_xor(int a, bool na, int b, bool nb) { add_or(a, na, b, nb); add_or(a, !na, b, !nb); }
    void add_xnor(int a, bool na, int b, bool nb) { add_or(a, na, b, !nb); add_or(a, !na, b, nb); }
    void force(int a, bool na) { int u = get_id(a, na); add_edge(u ^ 1, u); }

    void dfs1(int u) {
        vis[u] = true;
        for (int v : adj[u]) if (!vis[v]) dfs1(v);
        order.push_back(u);
    }

    void dfs2(int u, int c) {
        comp[u] = c;
        for (int v : radj[u]) if (comp[v] == -1) dfs2(v, c);
    }

    bool solve() {
        vis.assign(2 * n, false); order.clear();
        for (int i = 0; i < 2 * n; ++i) if (!vis[i]) dfs1(i);
        comp.assign(2 * n, -1);
        int c = 0;
        for (int i = 2 * n - 1; i >= 0; --i) {
            int u = order[i];
            if (comp[u] == -1) dfs2(u, c++);
        }
        for (int i = 0; i < n; ++i) {
            if (comp[i << 1] == comp[(i << 1) | 1]) return false;
            ans[i] = comp[i << 1] > comp[(i << 1) | 1];
        }
        return true;
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

