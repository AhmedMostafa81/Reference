#include <bits/stdc++.h>
using namespace std;

// Hungarian Algorithm - O(max(N, M)^3)
// Solves Minimum Cost Bipartite Matching (Assignment Problem)
template <typename T>
pair<T, vector<int>> hungarian(const vector<vector<T>>& cost_matrix) {
    if (cost_matrix.empty()) return {0, {}};
    
    int n = cost_matrix.size();
    int m = cost_matrix[0].size();
    
    // Pad to a square matrix K x K internally to handle N != M seamlessly
    int k = max(n, m);
    vector<vector<T>> a(k + 1, vector<T>(k + 1, 0));
    const T INF = numeric_limits<T>::max() / 2;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            a[i + 1][j + 1] = cost_matrix[i][j];
        }
    }

    vector<T> u(k + 1, 0), v(k + 1, 0);
    vector<int> p(k + 1, 0), way(k + 1, 0);

    for (int i = 1; i <= k; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<T> minv(k + 1, INF);
        vector<bool> used(k + 1, false);

        // Find augmenting path
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            T delta = INF;

            for (int j = 1; j <= k; ++j) {
                if (!used[j]) {
                    T cur = a[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }

            // Update potentials
            for (int j = 0; j <= k; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);

        // Augment the path
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }

    // Prepare result
    T min_cost = -v[0];
    vector<int> assignment(n, -1);
    for (int j = 1; j <= k; ++j) {
        if (p[j] != 0 && p[j] <= n && j <= m) {
            assignment[p[j] - 1] = j - 1; // Convert back to 0-indexed
        }
    }

    return {min_cost, assignment};
}

void pewpew() {
    int n, m; 
    cin >> n >> m; // e.g., N workers, M jobs
    
    // cost[worker][job]
    vector<vector<long long>> cost(n, vector<long long>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> cost[i][j];
        }
    }

    auto [min_cost, assignment] = hungarian(cost);

    cout << "Minimum Cost: " << min_cost << '\n';
    for (int i = 0; i < n; i++) {
        cout << "Worker " << i << " gets job " << assignment[i] << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    pewpew();
    return 0;
}
