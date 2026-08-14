#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;

// Compact, flexible XOR basis (linear basis) for competitive programming.
// - Stores a basis with one vector per bit position (0..63).
// - Insert is O(LOG). Queries are O(LOG).
// - Provided operations: insert, canRepresent, getMin, getMax, getMaxSubset,
//   getKth, merge, getBasis, rank, countDistinct, normalize.

struct XorBasis {
    static const int LOG = 64;            // change to 32 for 32-bit numbers
    array<u64, LOG> b;                    // b[i] has pivot at bit i (if non-zero)
    int rnk;                              // Tracks the dimension of the basis

    XorBasis() { clear(); }

    // Clear the basis.
    void clear() { 
        b.fill(0); 
        rnk = 0; 
    }

    // Insert x into the basis. Returns true if x increased the rank (was independent).
    bool insert(u64 x) {
        for (int i = LOG - 1; i >= 0; --i) {
            if ((x >> i) & 1ULL) {
                if (!b[i]) {
                    b[i] = x;
                    rnk++;
                    return true;
                }
                x ^= b[i];
            }
        }
        return false;
    }

    // Minimizes (x XOR s) over all s in the span of the basis.
    // Extremely useful for finding the XOR sum closest to a target 'x'.
    u64 getMin(u64 x = 0) const {
        for (int i = LOG - 1; i >= 0; --i) {
            if (b[i] && (x ^ b[i]) < x) {
                x ^= b[i];
            }
        }
        return x;
    }

    // Check whether x can be represented as xor of a subset of inserted numbers.
    bool canRepresent(u64 x) const {
        return getMin(x) == 0;
    }

    // Maximize (x XOR s) over all s in the span of the basis and return the value.
    u64 getMax(u64 x = 0) const {
        for (int i = LOG - 1; i >= 0; --i) {
            if (b[i] && (x ^ b[i]) > x) {
                x ^= b[i];
            }
        }
        return x;
    }

    // Maximum xor obtainable by xor-ing any subset of inserted numbers.
    u64 getMaxSubset() const { 
        return getMax(0); 
    }

    // K-th smallest distinct XOR sum (0-indexed). 0 is the 0-th element.
    // Returns -1ULL if k is out of bounds (k >= 2^rank).
    u64 getKth(u64 k) const {
        if (k >= countDistinct()) return -1ULL;
        
        vector<u64> v;
        for (int i = 0; i < LOG; ++i) {
            if (b[i]) v.push_back(b[i]);
        }
        
        // Local normalization to Reduced Row-Echelon Form. 
        // Ensures v[i] has a unique MSB not affecting other basis elements.
        for (size_t i = 0; i < v.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if ((v[i] ^ v[j]) < v[i]) v[i] ^= v[j];
            }
        }
        
        u64 res = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if ((k >> i) & 1ULL) res ^= v[i];
        }
        return res;
    }

    // Merge another basis into this (in-place). Returns added rank.
    int merge(const XorBasis &other) {
        int added = 0;
        for (int i = 0; i < LOG; ++i) {
            if (other.b[i]) added += insert(other.b[i]);
        }
        return added;
    }

    // Return non-zero basis vectors (unsorted). Useful for iterating or saving.
    vector<u64> getBasis() const {
        vector<u64> res;
        for (int i = 0; i < LOG; ++i) {
            if (b[i]) res.push_back(b[i]);
        }
        return res;
    }

    // Number of basis vectors (rank). O(1) time.
    int rank() const { return rnk; }

    // Number of distinct subset-xors = 2^rank.
    u64 countDistinct() const {
        if (rnk >= 64) return -1ULL; // Handle full 64-bit space safely
        return 1ULL << rnk;
    }

    // Convert basis to reduced row-echelon form in-place.
    void normalize() {
        for (int i = LOG - 1; i >= 0; --i) {
            if (b[i]) {
                for (int j = 0; j < LOG; ++j) {
                    if (j != i && b[j] && ((b[j] >> i) & 1ULL)) {
                        b[j] ^= b[i];
                    }
                }
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    XorBasis xb;
    vector<u64> a = {17ULL, 97ULL, 41ULL, 37ULL, 11ULL};
    for (auto v : a) xb.insert(v);

    assert(xb.canRepresent(0) == true);
    for (auto v : a) assert(xb.canRepresent(v));

    u64 mx = xb.getMaxSubset();
    for (auto v : a) assert(mx >= v);
    
    // Testing getKth (k=0 should be 0)
    assert(xb.getKth(0) == 0);

    XorBasis yb;
    yb.insert(5);
    int add = xb.merge(yb);
    (void)add; 

    return 0;
}
