#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;
const int BITS = 30; // High enough for integers up to ~10^9 (2^30)
const int SZ = (MAXN * BITS) + 5; 

struct PersistentTrie {
    int ptr = 1;
    int ch[SZ][2]; // Left child (0), Right child (1)
    int cnt[SZ];   // Number of elements passing through this node
    int roots[MAXN];

    void init() {
        ptr = 1;
        ch[0][0] = ch[0][1] = cnt[0] = 0;
        roots[0] = 0;
    }

    // Inserts integer `val` into version `old_root`, creates new node version
    int insert(int old_root, int val) {
        int new_root = ptr++;
        int curr = new_root;
        int old = old_root;

        cnt[curr] = cnt[old] + 1; // Copy and increment passing count

        for (int i = BITS - 1; i >= 0; i--) {
            int bit = (val >> i) & 1;
            
            // Link the unmodified branch to the old version
            ch[curr][!bit] = ch[old][!bit];

            // Create a brand new node for the modified branch
            ch[curr][bit] = ptr++;

            // Step deeper into both version trees
            curr = ch[curr][bit];
            old = ch[old][bit];

            cnt[curr] = cnt[old] + 1;
        }

        return new_root;
    }

    // Query 1: Maximize XOR with `val` in range [L, R]
    // L_root is root of version (L - 1), R_root is root of version (R)
    int query_max_xor(int L_root, int R_root, int val) {
        int res = 0;
        for (int i = BITS - 1; i >= 0; i--) {
            int bit = (val >> i) & 1;
            int desired = !bit; // Best-case bit to maximize XOR

            // Check if desired branch has at least one element added between versions L and R
            if (cnt[ch[R_root][desired]] - cnt[ch[L_root][desired]] > 0) {
                res |= (1 << i);
                R_root = ch[R_root][desired];
                L_root = ch[L_root][desired];
            } else {
                // Fallback to the other branch
                R_root = ch[R_root][bit];
                L_root = ch[L_root][bit];
            }
        }
        return res;
    }

    // Query 2: Count numbers in range [L, R] whose XOR with `val` is <= K
    int query_xor_less_than_k(int L_root, int R_root, int val, int k) {
        int ans = 0;
        for (int i = BITS - 1; i >= 0; i--) {
            int val_bit = (val >> i) & 1;
            int k_bit = (k >> i) & 1;

            if (k_bit == 1) {
                // If k has a 1 bit here, taking the matching branch gives XOR bit = 0, which is strictly < k
                int matching_branch = val_bit;
                ans += cnt[ch[R_root][matching_branch]] - cnt[ch[L_root][matching_branch]];

                // Move down the branch where XOR bit = 1 to continue matching remaining bits
                R_root = ch[R_root][!val_bit];
                L_root = ch[L_root][!val_bit];
            } else {
                // If k has a 0 bit here, we MUST take the matching branch to match 0
                R_root = ch[R_root][val_bit];
                L_root = ch[L_root][val_bit];
            }
        }
        // Include exact matches (XOR == K)
        ans += cnt[R_root] - cnt[L_root];
        return ans;
    }
};




#include <iostream>
#include <vector>

using namespace std;

// ... [Paste the PersistentTrie struct here] ...
PersistentTrie trie;

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    // Example: n elements, q queries
    if (!(cin >> n >> q)) return 0;

    vector<int> a(n + 1);
    
    // 1. Initialize the empty trie (Version 0)
    trie.init();

    // 2. Build the persistent versions (1-based indexing is highly recommended)
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        // Create version i by inserting a[i] into version (i - 1)
        trie.roots[i] = trie.insert(trie.roots[i - 1], a[i]);
    }

    // 3. Answer Queries
    while (q--) {
        int L, R, X;
        cin >> L >> R >> X; // 1-based indices: L and R

        // Query the range [L, R]
        // We pass the root just BEFORE L (which is L - 1) and the root at R
        int max_xor = trie.query_max_xor(trie.roots[L - 1], trie.roots[R], X);
        
        cout << max_xor << "\n";
    }

    return 0;
}
