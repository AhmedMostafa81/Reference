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
    // 0 based (right INCLUSIVE)
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

