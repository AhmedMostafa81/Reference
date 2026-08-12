// USE CASE: Point updates, Range sums/frequencies.
// TRICK: For any index i, find the nearest or farthest R such that the range [i, R] 
// contains at most k distinct numbers, in O(log n) time.
// (Usually done by mapping each value to its latest index, updating the tree at that index, 
// and using the roots history to query sums).


// pointers


const int N = ;
int fr[N];

struct Node {
    Node *l , *r;
    int sum = 0;
    Node(int val):l(nullptr) , r(nullptr) , sum(val){}
    Node(Node *l ,Node *r) :l(l) , r(r) , sum(0) {
        if (l) sum += l -> sum;
        if (r) sum += r -> sum;
    }
};

struct persistent_segment_tree {
    Node* build(int st ,int ed) {
        if (st == ed)
            return new Node(fr[st]);
        int mid = (st + ed) >> 1;
        return new Node(build(st , mid) , build(mid + 1 , ed));
    }
    Node* update(Node* v, int st , int ed , int pos , int new_fr) {
        if (st == ed)
            return new Node(new_fr);
        int mid = (st + ed) >> 1;
        if (pos <= mid)
            return new Node(update(v->l , st , mid , pos , new_fr) ,v->r );
        else
            return new Node(v->l , update(v->r , mid + 1 , ed , pos , new_fr)  );
    }
    int query(Node* L , Node* R , int st , int ed , int k) {
        if (st >= k)
            return R->sum - L->sum;
        if (ed < k)
            return 0;
        int mid = (st + ed) >> 1;
        return query(L->l , R->l , st , mid , k) + query(L -> r , R -> r , mid + 1 , ed , k );
    }
};


    vector<Node*>v = {seg.build(0,n+5)};






// without pointers     (faster)

#define M ((st + ed) >> 1)

const int MAXN = 200005;
const int MAXQ = 200005;
const int UPDATES_PER_QUERY = 3; // Number of updates performed per query
const int MAX_UPDATES = MAXQ * UPDATES_PER_QUERY;
const int LOG = 20;

// Base tree build + total node allocations from all updates
const int SZ = (MAXN * 4) + (MAX_UPDATES * LOG) + 5;

int ptr = 1;
int lc[SZ], rc[SZ];
long long sum[SZ];
int roots[MAXQ]; // Store roots for each version (or update query)

void init_segment_tree() {
    ptr = 1;
    lc[0] = rc[0] = sum[0] = 0; 
}

int build(int st, int ed) {
    if (st == ed) return 0; // return leaf node if reading from array
    int p = ptr++;
    lc[p] = build(st, M);
    rc[p] = build(M + 1, ed);
    return p;
}

int update(int old, int st, int ed, int pos, int val) {
    int p = ptr++;
    lc[p] = lc[old]; rc[p] = rc[old]; 
    sum[p] = sum[old] + val; 
    
    if (st == ed) return p;
    
    if (pos <= M) lc[p] = update(lc[old], st, M, pos, val);
    else rc[p] = update(rc[old], M + 1, ed, pos, val);
    
    return p;
}

long long query(int L_node, int R_node, int st, int ed, int k) {
    if (st >= k) return sum[R_node] - sum[L_node];
    if (ed < k) return 0;
    
    return query(lc[L_node], lc[R_node], st, M, k) + 
           query(rc[L_node], rc[R_node], M + 1, ed, k);
}





// max subarray sum
// USE CASE: Complex node merging (Max Subarray Sum, Longest Increasing Subsequence, etc.)
// MODIFICATION: Just change 'Node' and 'merge' function. The tree functions stay exactly the same.

#define M ((st + ed) >> 1)

const int MAXN = 200005;
const int MAXQ = 200005;
const int UPDATES_PER_QUERY = 3; // Number of updates performed per query
const int MAX_UPDATES = MAXQ * UPDATES_PER_QUERY;
const int LOG = 20;

// Base tree build + total node allocations from all updates
const int SZ = (MAXN * 4) + (MAX_UPDATES * LOG) + 5;

int ptr = 1;
int lc[SZ], rc[SZ];
int roots[MAXQ];

struct Node {
    long long sum, pref, suf, mx;
    int len;

    Node(long long v = 0, int l = 0) { 
        sum = v;
        pref = suf = mx = max(0LL, v);
        len = l;
    }

    friend Node merge(const Node &a, const Node &b) {
        if (!a.len) return b;
        if (!b.len) return a;
        Node c;
        c.len = a.len + b.len;
        c.sum = a.sum + b.sum;
        c.pref = max(a.pref, a.sum + b.pref);
        c.suf  = max(b.suf, b.sum + a.suf);
        c.mx   = max({a.mx, b.mx, a.suf + b.pref});
        return c;
    }
} seg[SZ];

void init_segment_tree() {
    ptr = 1;
    lc[0] = rc[0] = 0;
    seg[0] = Node(); 
}

int build(int st, int ed) {
    int p = ptr++;
    if (st == ed) {
        seg[p] = Node(-2e9, 1); 
        return p;
    }
    lc[p] = build(st, M);
    rc[p] = build(M + 1, ed);
    seg[p] = merge(seg[lc[p]], seg[rc[p]]);
    return p;
}

int update(int old, int st, int ed, int pos, int val) {
    int p = ptr++;
    if (st == ed) {
        seg[p] = Node(val, 1);
        return p;
    }
    
    lc[p] = lc[old]; rc[p] = rc[old];
    
    if (pos <= M) lc[p] = update(lc[old], st, M, pos, val);
    else rc[p] = update(rc[old], M + 1, ed, pos, val);
    
    seg[p] = merge(seg[lc[p]], seg[rc[p]]);
    return p;
}

Node query(int node, int st, int ed, int l, int r) {
    if (r < st || l > ed || !node) return Node(); 
    if (l <= st && ed <= r) return seg[node];
    
    if (r <= M) return query(lc[node], st, M, l, r);
    if (l > M) return query(rc[node], M + 1, ed, l, r);
    
    return merge(query(lc[node], st, M, l, r), 
                 query(rc[node], M + 1, ed, l, r));
}
