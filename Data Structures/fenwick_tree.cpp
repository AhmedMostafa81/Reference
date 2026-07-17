// USE CASE: Point Updates, Range Queries.
// TRICK: Includes Binary Lifting to find the K-th smallest element in O(log N)
// WARNING: Fenwick Trees MUST be 1-indexed. Index 0 will infinite loop!

const int MAXN = 200005;
const int LOG = 20; // log2(MAXN)

long long bit[MAXN];

void add(int idx, long long val) {
    for (; idx < MAXN; idx += idx & -idx)
        bit[idx] += val;
}

long long query(int idx) {
    long long sum = 0;
    for (; idx > 0; idx -= idx & -idx)
        sum += bit[idx];
    return sum;
}

// Sum of range [l, r]
long long query(int l, int r) {
    if (l > r) return 0;
    return query(r) - query(l - 1);
}

// Finds the 1-based index of the k-th smallest element (if elements are frequencies)
// Equivalent to finding the first index where prefix sum >= k
int get_kth(long long k) {
    int idx = 0;
    for (int i = LOG; i >= 0; i--) {
        int next_idx = idx + (1 << i);
        if (next_idx < MAXN && bit[next_idx] < k) {
            idx = next_idx;
            k -= bit[idx];
        }
    }
    return idx + 1;
}
