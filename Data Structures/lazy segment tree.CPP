const long long inf = 1e18; 
struct Node {
    long long mn = inf;
}neutral;
 
struct lazy_segment_tree {
    int sz;
    vector<Node> tree;
    vector<long long> lazy;

    void init(int n) {
        sz = 1;
        while (sz < n) sz *= 2;
        tree.assign(2 * sz, neutral);
        lazy.assign(2 * sz, 0);
    }

    Node merge(const Node& a, const Node& b) {
        Node rt;
        rt.mn = min(a.mn, b.mn);
        return rt;
    }

    void apply(int node, long long val) {
        tree[node].mn += val;
        lazy[node] += val;
    }

    void prop(int node, int s, int e) {
        if (!lazy[node] || e - s == 1) return; 
        
        apply(node * 2 + 1, lazy[node]);
        apply(node * 2 + 2, lazy[node]);
        
        lazy[node] = 0;
    }

    void set(int i, int v, int node, int s, int e) {
        if (e - s == 1) {
            tree[node].mn = v;
            lazy[node] = 0; 
            return;
        }
        prop(node, s, e); 
        
        int mid = (s + e) >> 1;
        if (i < mid)
            set(i, v, node * 2 + 1, s, mid);
        else        
            set(i, v, node * 2 + 2, mid, e);
        
        tree[node] = merge(tree[node * 2 + 1], tree[node * 2 + 2]);
    }

    void set(int i, int v) {
        set(i, v, 0, 0, sz);
    }

    void update(int l, int r, int val, int node, int s, int e) {
        if (s >= r || e <= l) return; 
        if (s >= l && e <= r) {  
            apply(node, val); 
            return;
        }
        prop(node, s, e); 
        
        int mid = (s + e) >> 1;
        update(l, r, val, node * 2 + 1, s, mid);
        update(l, r, val, node * 2 + 2, mid, e);
        
        tree[node] = merge(tree[node * 2 + 1], tree[node * 2 + 2]);
    }

    void update(int l, int r, int val) {
        update(l, r, val, 0, 0, sz);
    }

    Node query(int l, int r, int node, int s, int e) { 
        if (s >= r || e <= l) return neutral;
        if (s >= l && e <= r) return tree[node];
        
        prop(node, s, e);
        
        int mid = (s + e) >> 1;
        return merge(query(l, r, node * 2 + 1, s, mid), 
                     query(l, r, node * 2 + 2, mid, e));
    }

    long long query(int l, int r) {
        return query(l, r, 0, 0, sz).mn;
    }
};


// another version

// USE CASE: Range Sum + Range Add + Range Multiply
// MODIFICATION: To solve a different problem, ONLY edit Node, Tag, merge, compose, and apply_tag.

const int MAXN = 200005;

#define lc (node << 1)
#define rc ((node << 1) | 1)
#define mid ((st + ed) >> 1)

struct Node {
    long long sum = 0;
    // Neutral node definition (0 for sum, INF for min)
    Node() {} 
    Node(long long _sum) : sum(_sum) {}
};

struct Tag {
    long long mul = 1, add = 0;
    // Neutral tag definition
    Tag() {} 
    Tag(long long _mul, long long _add) : mul(_mul), add(_add) {}
    bool is_neutral() const { return mul == 1 && add == 0; }
};

Node tree[4 * MAXN];
Tag lazy[4 * MAXN];

// Combine two nodes
Node merge(const Node& a, const Node& b) {
    return Node(a.sum + b.sum);
}

// Combine an old lazy tag with a new lazy tag coming from above
Tag compose(const Tag& old_tag, const Tag& new_tag) {
    return Tag(old_tag.mul * new_tag.mul, old_tag.add * new_tag.mul + new_tag.add);
}

// Apply a tag to a node and update the node's answer
void apply_tag(int node, int st, int ed, const Tag& t) {
    long long len = ed - st + 1;
    tree[node].sum = tree[node].sum * t.mul + t.add * len;
    lazy[node] = compose(lazy[node], t);
}

void push(int node, int st, int ed) {
    if (!lazy[node].is_neutral()) {
        apply_tag(lc, st, mid, lazy[node]);
        apply_tag(rc, mid + 1, ed, lazy[node]);
        lazy[node] = Tag(); // Reset to neutral
    }
}

void build(int node, int st, int ed, const vector<long long>& a) {
    lazy[node] = Tag();
    if (st == ed) {
        tree[node] = Node(st < a.size() ? a[st] : 0);
        return;
    }
    build(lc, st, mid, a);
    build(rc, mid + 1, ed, a);
    tree[node] = merge(tree[lc], tree[rc]);
}

void update(int node, int st, int ed, int l, int r, const Tag& t) {
    if (l > ed || r < st) return;
    if (l <= st && ed <= r) {
        apply_tag(node, st, ed, t);
        return;
    }
    push(node, st, ed);
    update(lc, st, mid, l, r, t);
    update(rc, mid + 1, ed, l, r, t);
    tree[node] = merge(tree[lc], tree[rc]);
}

Node query(int node, int st, int ed, int l, int r) {
    if (l > ed || r < st) return Node(); // Neutral node
    if (l <= st && ed <= r) return tree[node];
    
    push(node, st, ed);
    return merge(query(lc, st, mid, l, r), 
                 query(rc, mid + 1, ed, l, r));
}

// Wrappers for clean usage in main()
void range_add(int l, int r, long long x, int n) { update(1, 1, n, l, r, Tag(1, x)); }
void range_mul(int l, int r, long long x, int n) { update(1, 1, n, l, r, Tag(x, 0)); }









