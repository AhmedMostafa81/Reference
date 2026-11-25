// Assign , Add , Query sum
const int N = ;

long long tree[N] ;
long long lazy[2][N];
#define mid ((s + e) >> 1)
#define L  (node << 1 | 1)
#define R (L + 1)

void prop(int node,  int s , int e) {
    if (lazy[0][node] != -1) { // assign
        tree[node] = 1ll * lazy[0][node] * (e - s);
        if (s != e - 1) {
            lazy[0][L] = lazy[0][R] = lazy[0][node];
            lazy[1][L] = lazy[1][R] = 0;
        }
    }// Add
        tree[node] += 1ll * lazy[1][node] * (e - s);
        if (s != e - 1) {
            lazy[1][L] += lazy[1][node];
            lazy[1][R] += lazy[1][node];
        }
    lazy[0][node] = -1;
    lazy[1][node] = 0;
}

void update(int l , int r , int node , int s, int e , int val , bool type) {
    prop(node , s , e);
    if (l >= e || r <= s)
        return ;
    if (s >= l && e <= r) {
        lazy[type][node] = val;
        prop(node , s , e);
        return ;
    }
    update(l , r , L , s , mid , val ,type);
    update(l , r , R , mid , e , val ,type);
    tree[node] = tree[L] + tree[R];
}

int n ;

void update(int l ,  int r , int val , bool type) {
    update(l , r , 0 , 0 , n + 2 , val , type);
}

long long query(int l , int r , int node , int s , int e) {
    prop(node , s , e);
    if (l >= e || r <= s)
        return 0;
    if (s >= l && e <= r)
        return tree[node];
    return query(l , r , L , s , mid) + query(l , r , R , mid , e);
}

long long query(int l , int r) {
    return query(l , r , 0 ,0  , n + 2);
}

void maybe() {

    memset(lazy[0] , -1 , sizeof lazy[0]);
    memset(lazy[1] , 0 , sizeof lazy[1]);

    int m ;cin >> n >> m ;
    while (m--) {
        int op; cin >> op;
        int l , r;cin >> l >> r;
        if (op == 1) {
            int v; cin >> v;
            update(l , r , v , 0); // assign
        }
        else if (op == 2) {
            int v;cin >> v;
            update(l , r , v , 1); // add
        }
        else {
            cout << query(l , r) << '\n';
        }
    }

}
