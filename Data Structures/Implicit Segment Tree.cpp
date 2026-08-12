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
