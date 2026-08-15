const int BIT = 30 ;
struct Trie {
    struct Node {
        int children[2] = {};
        int f = 0;
    };

    vector<Node> trie;

    Trie() {
        trie.emplace_back();
        // insert(0);
    }

    void insert(int x) {
        int idx = 0;
        for(int bit = BIT; bit >= 0; bit--) {
            int nxt = x >> bit & 1;

            if(trie[idx].children[nxt] == 0) {
                trie[idx].children[nxt] = trie.size();
                trie.emplace_back();
            }
            idx = trie[idx].children[nxt];
            trie[idx].f++;
        }
    }

    void erase(int x) {
        int idx = 0;
        for(int bit = BIT; bit >= 0; bit--) {
            int nxt = x >> bit & 1;

            idx = trie[idx].children[nxt];
            trie[idx].f--;
        }
    }

    int query(int x) { // max xor with x
        int idx = 0;
        int ret = 0;
        for(int bit = BIT; bit >= 0; bit--) {
            int nxt = !(x >> bit & 1);

            if (trie[idx].children[nxt] == 0 || trie[trie[idx].children[nxt]].f == 0)
                nxt ^= 1;
            else
                ret |= 1ll << bit;
            idx = trie[idx].children[nxt];
        }
        return ret;
    }

    void clear() {
        trie.clear();
        trie.emplace_back();
        // insert(0);
    }
};
