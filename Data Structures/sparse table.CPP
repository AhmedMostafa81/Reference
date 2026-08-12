struct ST { // sparse table version 1
    int n, lg;
    vector<vector<int>> t;
    vector<int> log;

    int merge(int a, int b){ return min(a,b); }

    ST(vector<int>& a){
        n = a.size();

        log.assign(n+1,0);
        for(int i=2;i<=n;i++)
            log[i]=log[i/2]+1;

        lg = log[n] + 1;

        t.assign(lg, vector<int>(n));

        for(int i=0;i<n;i++)
            t[0][i]=a[i];

        for(int j=1;j<lg;j++)
            for(int i=0;i+(1<<j)<=n;i++)
                t[j][i]=merge(t[j-1][i],
                              t[j-1][i+(1<<(j-1))]);
    }

    int get(int l,int r){
        int j=log[r-l+1];
        return merge(t[j][l], t[j][r-(1<<j)+1]);
    }

    // for suffix array        ==========================

     int get_left_less(int idx , int val) {
        int s = 0 , e = idx , mid , rt = -1;
        while (s <= e) {
            mid = (s + e) >> 1;
            if (get(mid , idx) < val)
                rt = mid , s = mid + 1;
            else
                e = mid - 1;
        }
        return rt;
    }

    int get_right_less(int idx , int val) {
        int s = idx , e = n-1 , mid , rt = n;
        while (s <= e) {
            mid = (s + e) >> 1;
            if (get(idx , mid) < val)
                rt = mid , e = mid - 1;
            else
                s = mid + 1;
        }
        return rt;
    }
    
};


// 2d sparse table 


struct SparseTable2D {
    int n, m;
    int LOGN, LOGM;
    vector<int> lg;
    
    vector<int> st;

    inline int idx(int x, int y, int i, int j) const {
        return ((x * LOGM + y) * n + i) * m + j;
    }

    inline int merge(int a, int b) const {
        return min(a, b);
    }

    SparseTable2D(const vector<vector<int>>& a) { // O(n * m * log(n) * log(m))
        if (a.empty() || a[0].empty()) return;
        
        n = a.size();
        m = a[0].size();

        lg.assign(max(n, m) + 1, 0);
        for (int i = 2; i < (int)lg.size(); i++)
            lg[i] = lg[i / 2] + 1;

        LOGN = lg[n] + 1;
        LOGM = lg[m] + 1;

        st.resize(LOGN * LOGM * n * m);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                st[idx(0, 0, i, j)] = a[i][j];
            }
        }

        for (int x = 0; x < LOGN; x++) {
            for (int y = 0; y < LOGM; y++) {
                if (x == 0 && y == 0) continue;

                for (int i = 0; i + (1 << x) <= n; i++) {
                    for (int j = 0; j + (1 << y) <= m; j++) {
                        
                        if (x) {
                            st[idx(x, y, i, j)] = merge(
                                st[idx(x - 1, y, i, j)],
                                st[idx(x - 1, y, i + (1 << (x - 1)), j)]
                            );
                        } else {
                            st[idx(x, y, i, j)] = merge(
                                st[idx(x, y - 1, i, j)],
                                st[idx(x, y - 1, i, j + (1 << (y - 1)))]
                            );
                        }
                        
                    }
                }
            }
        }
    }

    int query(int x1, int y1, int x2, int y2) const {  // O(1)
        int kx = lg[x2 - x1 + 1];
        int ky = lg[y2 - y1 + 1];

        int a1 = st[idx(kx, ky, x1, y1)];
        int a2 = st[idx(kx, ky, x2 - (1 << kx) + 1, y1)];
        int a3 = st[idx(kx, ky, x1, y2 - (1 << ky) + 1)];
        int a4 = st[idx(kx, ky, x2 - (1 << kx) + 1, y2 - (1 << ky) + 1)];

        return merge(merge(a1, a2), merge(a3, a4));
    }
};

