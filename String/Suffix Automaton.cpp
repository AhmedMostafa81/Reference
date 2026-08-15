#include <bits/stdc++.h>
using namespace std;

struct SuffixAutomaton {
    struct State {
        int len, link;
        int next[26];
        bool is_clone;
        
        // DP variables for answering queries
        long long cnt;      // Number of occurrences (endpos equivalence class size)
        int first_pos;      // 0-indexed ending position of the first occurrence
        long long paths;    // Number of distinct substrings extending from this state

        State() {
            len = 0; link = -1;
            memset(next, -1, sizeof(next));
            is_clone = false;
            cnt = 0; first_pos = -1; paths = 0;
        }
    };

    vector<State> st;
    int sz, last;

    // Initialize with maximum possible string length
    SuffixAutomaton(int max_len) {
        st.resize(2 * max_len);
        sz = 1;
        last = 0;
        st[0].len = 0;
        st[0].link = -1;
        st[0].first_pos = -1;
    }

    // 1. Build the Automaton
    void extend(char c) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1; 
        st[cur].first_pos = st[cur].len - 1; // 0-indexed position
        
        int p = last;
        while (p != -1 && st[p].next[c - 'a'] == -1) {
            st[p].next[c - 'a'] = cur;
            p = st[p].link;
        }
        
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c - 'a'];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone] = st[q];
                st[clone].len = st[p].len + 1;
                st[clone].is_clone = true;
                st[clone].cnt = 0; // Clones don't represent new occurrences directly
                st[clone].first_pos = st[q].first_pos; // Inherit first occurrence from original
                
                while (p != -1 && st[p].next[c - 'a'] == q) {
                    st[p].next[c - 'a'] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void build(const string &s) {
        for (char c : s) extend(c);
        compute_dp();
    }

    // 2. Precompute DP Data in O(N) using Counting Sort
    void compute_dp() {
        int max_l = st[last].len;
        vector<int> buckets(max_l + 1, 0);
        for (int i = 0; i < sz; i++) buckets[st[i].len]++;
        for (int i = 1; i <= max_l; i++) buckets[i] += buckets[i - 1];
        
        vector<int> order(sz);
        for (int i = 0; i < sz; i++) order[--buckets[st[i].len]] = i;

        // DP 1: Propagate occurrences up the link tree (leaves to root)
        for (int i = sz - 1; i >= 1; i--) {
            int u = order[i];
            int p = st[u].link;
            if (p != -1) {
                st[p].cnt += st[u].cnt;
            }
        }

        // DP 2: Propagate paths down the transition DAG (leaves to root on DAG)
        // Helps with lexicographical queries and total distinct substring counts
        for (int i = sz - 1; i >= 0; i--) {
            int u = order[i];
            st[u].paths = 1; // Count the empty string ending at this state
            for (int c = 0; c < 26; c++) {
                if (st[u].next[c] != -1) {
                    st[u].paths += st[st[u].next[c]].paths;
                }
            }
        }
    }

    // -----------------------------------------------------------------
    // QUERY FUNCTIONS (General Subproblems)
    // -----------------------------------------------------------------

    // Subproblem 1: Total distinct substrings
    long long distinct_substrings_count() {
        long long total = 0;
        for (int i = 1; i < sz; i++) {
            total += (st[i].len - st[st[i].link].len);
        }
        // Alternatively: return st[0].paths - 1; (subtract 1 for empty string)
        return total;
    }

    // Subproblem 2: Frequency of a specific pattern T
    long long pattern_frequency(const string &t) {
        int cur = 0;
        for (char c : t) {
            if (st[cur].next[c - 'a'] == -1) return 0;
            cur = st[cur].next[c - 'a'];
        }
        return st[cur].cnt;
    }

    // Subproblem 3: 0-indexed starting index of the FIRST occurrence of pattern T
    int first_occurrence(const string &t) {
        int cur = 0;
        for (char c : t) {
            if (st[cur].next[c - 'a'] == -1) return -1;
            cur = st[cur].next[c - 'a'];
        }
        return st[cur].first_pos - t.length() + 1;
    }

    // Subproblem 4: Lexicographically k-th distinct substring
    // Assumes distinct substrings (ignores how many times they appear). 
    // To weight by frequency, change `st[u].paths += ...` to `st[u].paths += st[next].paths + st[next].cnt`
    string kth_substring(long long k) {
        if (k >= st[0].paths) return ""; // Out of bounds
        
        string res = "";
        int cur = 0;
        while (k > 0) {
            for (int c = 0; c < 26; c++) {
                int nxt = st[cur].next[c];
                if (nxt != -1) {
                    if (k <= st[nxt].paths) {
                        res += (char)(c + 'a');
                        k -= 1; // 1 for the string ending exactly here
                        cur = nxt;
                        break;
                    } else {
                        k -= st[nxt].paths; // Skip all substrings starting with this branch
                    }
                }
            }
        }
        return res;
    }

    // Subproblem 5: Longest Common Substring with another string T
    int longest_common_substring(const string &t) {
        int cur = 0, current_len = 0, best = 0;
        for (char c : t) {
            while (cur != 0 && st[cur].next[c - 'a'] == -1) {
                cur = st[cur].link;
                current_len = st[cur].len;
            }
            if (st[cur].next[c - 'a'] != -1) {
                cur = st[cur].next[c - 'a'];
                current_len++;
            }
            best = max(best, current_len);
        }
        return best;
    }
};
