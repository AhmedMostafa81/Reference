
// O(Q log Q + (M + Q) * sqrt(M) * log N)
// you can change this line "int sq = sqrt(m) + 2;" to change complexity

#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
 
using namespace std;
 
const int N = 50005;
 
vector<tuple<int, int, int, int>> mo;
int U[N], V[N];
int L, R;
 
int parent_dsu[N], sz_dsu[N];
int comps;
 
struct Change {
    int u, v, comp_changed;
};
vector<Change> history;
 
void DSU_init(int n) {
    comps = n;
    for (int i = 1; i <= n; i++) {
        parent_dsu[i] = i;
        sz_dsu[i] = 1;
    }
    history.clear();
}
 
int find_set(int v) {
    while (v != parent_dsu[v]) v = parent_dsu[v];
    return v;
}
 
void add(int idx) {
    int a = find_set(U[idx]);
    int b = find_set(V[idx]);
    if (a != b) {
        if (sz_dsu[a] < sz_dsu[b]) swap(a, b);
        history.push_back({a, b, 1});
        parent_dsu[b] = a;
        sz_dsu[a] += sz_dsu[b];
        comps--;
    } else {
        history.push_back({-1, -1, 0});
    }
}
 
void rem() {
    if (history.empty()) return;
    auto ch = history.back();
    history.pop_back();
    if (ch.comp_changed) {
        sz_dsu[ch.u] -= sz_dsu[ch.v];
        parent_dsu[ch.v] = ch.v;
        comps++;
    }
}
 
void pewpew() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    int n, m; 
    if (!(cin >> n >> m)) return;
 
    for (int i = 0; i < m; i++) cin >> U[i] >> V[i];
 
    int q; cin >> q;
    int sq = sqrt(m) + 2; 
 
    for (int i = 0; i < q; i++) {
        int l, r; cin >> l >> r;
        l--, r--;
        mo.push_back({l / sq, r, l, i});
    }
 
    int ans[q];
    sort(mo.begin(), mo.end());
 
    int cur_block = -1;
 
    for (auto &[tt, r, l, idx] : mo) {
        // 1. When entering a new block, reset DSU and set pointers to baseline
        if (tt != cur_block) {
            DSU_init(n);
            cur_block = tt;
            R = (tt + 1) * sq - 1; 
            L = R + 1;             
        }
 
        // 2. Handle small queries (completely inside the block) separately
        if (r < (tt + 1) * sq) {
            for (int i = l; i <= r; i++) add(i);
            ans[idx] = comps;
            for (int i = l; i <= r; i++) rem();
            continue;
        }
 
        // 3. Your classic while loops (Adjusted safely for Rollback DSU)
        while (R < r) {
            add(++R); 
        }
        while (L > l) {
            add(--L); 
        }
 
        ans[idx] = comps;
 
        // 4. Roll back ONLY the L pointer modifications to prepare for the next query
        int baseline = (tt + 1) * sq;
        while (L < baseline) {
            rem();
            L++;
        }
    }
 
    for (int i = 0; i < q; i++) cout << ans[i] << '\n';
}
 
int main() {
    pewpew();
    return 0;
}
