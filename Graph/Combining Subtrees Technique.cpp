# Tree Knapsack DP Optimization Cheat Sheet

## 1. Core Concept
In tree knapsack DP problems  
(example: choosing exactly `K` nodes or counting connected subtrees of size `K`),
a naive solution loops from `0 -> K` during every merge:
for (i = 0; i <= K; i++)
    for (j = 0; j <= K; j++)
Complexity: `O(N * K^2)`
This is often too slow.
  
## The Optimization

Instead of looping to `K` every time,
limit the loops using current subtree sizes:

for (i = 1; i <= min(K, sz[u]); i++)
    for (j = 1; j <= min(K - i, sz[v]); j++)

Complexity: `O(N * K)`

## Why It Works

Each pair of nodes from different subtrees is merged only once,
when their LCA is processed.
So total operations stay globally bounded.

# 2. Golden Rules

### 1) Solve Child First
dfs(v, u);
before merging.
### 2) Use Temporary Array
Always merge into:
vector<long long> new_dp(K + 1, 0);
Do NOT modify `dp[u]` directly while iterating.
### 3) Update Size After Merge
Correct:
sz[u] += sz[v];
AFTER loops finish.

dp[u][k] == Within the subtree of node u, how many ways can I build a connected component of exactly size k



# 3. Standard Template (C++)

    void dfs(int u, int p) {

        sz[u] = 1;

        // Base Case
        dp[u][1] = 1;

        for (int v : adj[u]) {

            if (v == p)
                continue;

            // 1) Solve child first
            dfs(v, u);

            // 2) Temporary buffer
            vector<long long> new_dp(K + 1, 0);

            // 3) Optimized merge
            for (int i = 1; i <= min(K, sz[u]); i++) {

                if (!dp[u][i])
                    continue;

                // Don't merge child
                new_dp[i] =
                    (new_dp[i] + dp[u][i]) % MOD;

                // Merge child
                for (int j = 1; j <= min(K - i, sz[v]); j++) {

                    if (!dp[v][j])
                        continue;

                    long long ways =
                        (dp[u][i] * dp[v][j]) % MOD;

                    new_dp[i + j] =
                        (new_dp[i + j] + ways) % MOD;
                }
            }

            // 4) Copy back
            for (int i = 1; i <= K; i++)
                dp[u][i] = new_dp[i];

            // IMPORTANT
            sz[u] += sz[v];
        }

        // Collect global answers
        for (int i = 1; i <= K; i++) {

            total_of_size[i] =
                (total_of_size[i] + dp[u][i]) % MOD;
        }
    }

  
# 4. Main Idea

The important optimization is:

for (int i = 1; i <= min(K, sz[u]); i++)
    for (int j = 1; j <= min(K - i, sz[v]); j++)

instead of:

for (int i = 1; i <= K; i++)
    for (int j = 1; j <= K; j++)

This small change converts the solution from `O(N * K^2)` to `O(N * K)`.
