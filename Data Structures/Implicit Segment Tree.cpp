// If you have Q queries over a range of [1, 10^9], max depth is ~30.
// Maximum nodes created = Q * 30. 
const int MAX_QUERIES = 200005;
const int LOG = 32; 
const int SZ = (MAX_QUERIES * LOG) + 5;

struct ImplicitSegTree {
    int ptr = 1;
    int root = 0; // 0 means no root yet (empty tree)
    
    int lc[SZ];
    int rc[SZ];
    long long sum[SZ];

    void init() {
        ptr = 1;
        root = 0;
        lc[0] = rc[0] = sum[0] = 0; // Node 0 is the universal "empty" null node
    }

    // Pass `node` by REFERENCE (&) so we can modify the parent's lc/rc directly
    void update(int& node, long long s, long long e, long long i, long long val) {
        if (!node) {
            node = ptr++;
            lc[node] = rc[node] = sum[node] = 0; // Initialize new node cleanly
        }
        
        sum[node] += val; // Apply point update going down
        
        if (s == e) return;
        
        long long mid = (s + e) >> 1;
        if (i <= mid) {
            update(lc[node], s, mid, i, val);
        } else {
            update(rc[node], mid + 1, e, i, val);
        }
    }

    long long query(int node, long long s, long long e, long long l, long long r) {
        // If we hit a non-existent node, or out of bounds, return 0 (neutral element)
        if (!node || s > r || e < l) return 0;
        
        if (s >= l && e <= r) return sum[node];
        
        long long mid = (s + e) >> 1;
        return query(lc[node], s, mid, l, r) + 
               query(rc[node], mid + 1, e, l, r);
    }
};




#include <iostream>

using namespace std;

// ... [Paste the ImplicitSegTree struct here] ...
ImplicitSegTree tree;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int q;
    if (!(cin >> q)) return 0;

    tree.init();
    
    // The domain of our problem. 
    // Must be long long to avoid overflow when doing (lx + rx) >> 1
    const long long MIN_X = 1;
    const long long MAX_X = 1e9; 

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            // Point Update: Add `val` to coordinate `pos`
            long long pos, val;
            cin >> pos >> val;
            
            // Note: We pass tree.root, and because it's passed by reference in 
            // the struct, the very first update will assign tree.root = 1.
            tree.update(tree.root, MIN_X, MAX_X, pos, val);
        } 
        else if (type == 2) {
            // Range Query: Sum from coordinate `L` to `R`
            long long L, R;
            cin >> L >> R;
            
            long long ans = tree.query(tree.root, MIN_X, MAX_X, L, R);
            cout << ans << "\n";
        }
    }

    return 0;
}
