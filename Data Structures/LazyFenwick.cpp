#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct LazyFenwick {
    int n;
    // bit1 maintains the difference array
    // bit2 maintains the multiples to adjust the prefix sums
    vector<T> bit1, bit2;

    LazyFenwick(int _n) : n(_n) {
        bit1.assign(n + 1, 0);
        bit2.assign(n + 1, 0);
    }

    // Internal point update
    void _add(vector<T>& bit, int idx, T val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    // Internal prefix query
    T _query(vector<T>& bit, int idx) {
        T sum = 0;
        for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
        return sum;
    }

    // RANGE ADD: Add 'val' to all elements in range [L, R]
    void add(int L, int R, T val) {
        if (L > R) return;
        _add(bit1, L, val);
        _add(bit1, R + 1, -val);
        _add(bit2, L, val * (L - 1));
        _add(bit2, R + 1, -val * R);
    }

    // PREFIX SUM: Get the sum of elements in range [1, idx]
    T query(int idx) {
        return _query(bit1, idx) * idx - _query(bit2, idx);
    }

    // RANGE SUM: Get the sum of elements in range [L, R]
    T query(int L, int R) {
        if (L > R) return 0;
        return query(R) - query(L - 1);
    }

    // POINT QUERY: Get the exact value at a single index
    T point_query(int idx) {
        return query(idx, idx);
    }

    // SET VALUE: Overwrite the value at an index (Useful for your "reset to 0" logic)
    void set_value(int idx, T target_val) {
        T current_val = point_query(idx);
        add(idx, idx, target_val - current_val);
    }
};
