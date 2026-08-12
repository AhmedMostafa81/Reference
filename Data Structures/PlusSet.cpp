
using ull = unsigned long long;

struct PlusSet {
    int n;
    std::vector<ull> a;

    Bitset(int n = 0) : n(n), a((n + 63) / 64, 0) {}

    void set(int i)   { a[i / 64] |= (1ULL << (i % 64)); }
    void reset(int i) { a[i / 64] &= ~(1ULL << (i % 64)); }
    void flip(int i)  { a[i / 64] ^= (1ULL << (i % 64)); }
    int test(int i) const { return (a[i / 64] >> (i % 64)) & 1; }

    // Cleans garbage bits in the last block (crucial for count, ==, and shifts)
    void clean() {
        if (n % 64 != 0) a.back() &= (1ULL << (n % 64)) - 1;
    }

    // ==========================================
    Bitset& operator|=(const Bitset& o) {
        for (size_t i = 0; i < a.size(); i++) a[i] |= o.a[i];
        return *this;
    }
    
    Bitset& operator&=(const Bitset& o) {
        for (size_t i = 0; i < a.size(); i++) a[i] &= o.a[i];
        return *this;
    }
    
    Bitset& operator^=(const Bitset& o) {
        for (size_t i = 0; i < a.size(); i++) a[i] ^= o.a[i];
        return *this;
    }

    // ==========================================
    Bitset& operator<<=(int k) {
        if (k >= n) { clear(); return *this; }
        int blocks = k / 64, bits = k % 64;
        
        // Go backwards to avoid overwriting data we still need
        for (int i = (int)a.size() - 1; i >= blocks; i--) {
            a[i] = a[i - blocks] << bits;
            if (bits > 0 && i - blocks - 1 >= 0) {
                a[i] |= (a[i - blocks - 1] >> (64 - bits));
            }
        }
        for (int i = 0; i < blocks; i++) a[i] = 0; // zero out lower blocks
        clean();
        return *this;
    }

    Bitset& operator>>=(int k) {
        if (k >= n) { clear(); return *this; }
        int blocks = k / 64, bits = k % 64;
        
        // Go forwards
        for (int i = 0; i + blocks < (int)a.size(); i++) {
            a[i] = a[i + blocks] >> bits;
            if (bits > 0 && i + blocks + 1 < (int)a.size()) {
                a[i] |= (a[i + blocks + 1] << (64 - bits));
            }
        }
        for (int i = (int)a.size() - blocks; i < (int)a.size(); i++) a[i] = 0;
        clean();
        return *this;
    }

    // ==========================================
    void clear() {
        std::fill(a.begin(), a.end(), 0ULL);
    }

    int count() const {
        int cnt = 0;
        for (ull x : a) cnt += __builtin_popcountll(x);
        return cnt;
    }

    bool operator==(const Bitset& o) const {
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i] != o.a[i]) return false;
        }
        return true;
    }

    // Fast iteration: Use like `for(int i = b.find_first(); i != -1; i = b.find_next(i))`
    int find_first() const {
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i]) return i * 64 + __builtin_ctzll(a[i]);
        }
        return -1;
    }

    int find_next(int i) const {
        i++;
        if (i >= n) return -1;
        int b = i / 64, bit = i % 64;
        
        // Check remainder of the current block
        ull masked = a[b] & (~0ULL << bit);
        if (masked) return b * 64 + __builtin_ctzll(masked);
        
        // Check subsequent blocks
        for (size_t j = b + 1; j < a.size(); j++) {
            if (a[j]) return j * 64 + __builtin_ctzll(a[j]);
        }
        return -1;
    }
};
