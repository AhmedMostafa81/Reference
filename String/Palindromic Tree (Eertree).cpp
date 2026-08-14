#include <bits/stdc++.h>
using namespace std;

// Palindromic Tree (Eertree) - O(N) time and space
// Stores all distinct palindromes and computes their exact frequencies.
struct Eertree {
    struct Node {
        int len;          // Length of the palindrome at this node
        int link;         // Suffix link: longest palindromic proper suffix
        int next[26];     // Edges for adding characters
        int end_pos;      // Ending index in the string where this palindrome first appeared
        long long cnt;    // Frequency of this palindrome in the string

        Node(int l = 0, int lk = 0) {
            len = l;
            link = lk;
            memset(next, 0, sizeof(next));
            end_pos = -1;
            cnt = 0;
        }
    };

    string S;
    vector<Node> tree;
    int sz;               // Total number of nodes
    int last;             // Node of the longest palindromic suffix of the processed prefix

    Eertree(const string& s) {
        S = s;
        int n = S.length();
        
        // A string of length N has at most N distinct palindromes, plus 2 roots
        tree.resize(n + 3); 
        
        // Node 1: Root for odd length palindromes
        tree[1] = Node(-1, 1); 
        // Node 2: Root for even length palindromes
        tree[2] = Node(0, 1);  
        
        sz = 2;
        last = 2;

        for (int i = 0; i < n; i++) {
            add_char(i);
        }
    }

    void add_char(int i) {
        int c = S[i] - 'a';
        int curr = last;

        // Traverse suffix links to find the largest palindrome that can be expanded
        while (i - 1 - tree[curr].len < 0 || S[i - 1 - tree[curr].len] != S[i]) {
            curr = tree[curr].link;
        }

        // If this distinct palindrome doesn't exist yet, create it
        if (tree[curr].next[c] == 0) {
            sz++;
            tree[sz].len = tree[curr].len + 2;
            tree[sz].end_pos = i;
            
            // Find the suffix link for the new node
            if (tree[sz].len == 1) {
                tree[sz].link = 2;
            } else {
                int lk = tree[curr].link;
                while (i - 1 - tree[lk].len < 0 || S[i - 1 - tree[lk].len] != S[i]) {
                    lk = tree[lk].link;
                }
                tree[sz].link = tree[lk].next[c];
            }
            tree[curr].next[c] = sz;
        }
        
        // Update last and increment the frequency for the current prefix
        last = tree[curr].next[c];
        tree[last].cnt++;
    }

    // Propagate the lazy frequency counts down the suffix links.
    // Must be called exactly once after the entire string is processed.
    void propagate_frequencies() {
        // Nodes are created strictly after the nodes they link to,
        // so iterating backwards guarantees we process larger palindromes first.
        for (int i = sz; i >= 3; i--) {
            tree[tree[i].link].cnt += tree[i].cnt;
        }
    }

    // Returns a vector of pairs: {palindrome_string, frequency}
    vector<pair<string, long long>> get_palindromes_with_frequencies() {
        propagate_frequencies();
        
        vector<pair<string, long long>> result;
        for (int i = 3; i <= sz; i++) {
            int length = tree[i].len;
            int start_idx = tree[i].end_pos - length + 1;
            result.push_back({S.substr(start_idx, length), tree[i].cnt});
        }
        return result;
    }
};

void pewpew() {
    string s;
    cin >> s; 
    // Example input: abaaba
    
    Eertree pt(s);
    vector<pair<string, long long>> distinct_pals = pt.get_palindromes_with_frequencies();
    
    for (const auto& [pal, freq] : distinct_pals) {
        cout << pal << " : " << freq << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    pewpew();
    return 0;
}
