#include <bits/stdc++.h>
using namespace std;

const int N = 200005, LOG = 20;
vector<int> gr[N];
int in[N], out[N], tim, up[N][LOG];
long long val[N], mx[N][LOG];

void pre(int node = 1, int par = 1){
    in[node] = ++tim;
    up[node][0] = par;
    mx[node][0] = max(val[node], val[par]); // Inclusive segment

    for (int i = 1; i < LOG; i++) {
        up[node][i] = up[up[node][i-1]][i-1];
        mx[node][i] = max(mx[node][i-1], mx[up[node][i-1]][i-1]);
    }

    for (auto ch : gr[node]){
        if (ch == par) continue;
        pre(ch, node);
    }
    out[node] = ++tim;
}

bool if_anc(int x, int y){
    return in[x] <= in[y] && out[x] >= out[y];
}

int LCA(int x, int y){
    if (if_anc(x, y)) return x;
    if (if_anc(y, x)) return y;

    for (int i = LOG - 1; i >= 0; i--)
        if (!if_anc(up[x][i], y))
            x = up[x][i];

    return up[x][0];
}

long long get_max_up(int u, int anc) {
    if (u == anc) return val[u];
    long long res = val[u];
    
    for (int i = LOG - 1; i >= 0; i--) {
        if (!if_anc(up[u][i], anc)) {
            res = max(res, mx[u][i]);
            u = up[u][i];
        }
    }
    return max(res, mx[u][0]);
}

long long query_max(int u, int v) {
    int lca = LCA(u, v);
    return max(get_max_up(u, lca), get_max_up(v, lca));
}

// another version

#include <bits/stdc++.h>
using namespace std;

const int N = 200005, LOG = 20;
vector<int> gr[N];
int in[N], out[N], tim, up[N][LOG];

struct Node {
    long long val = 0;
    int len = 0; 
} val[N], up_val[N][LOG], dn_val[N][LOG];

Node combine(Node a, Node b) {
    if (a.len == 0) return b; // Identity check
    if (b.len == 0) return a;
    
    // REPLACE THIS with Hash, Matrix Mult, etc.
    return {a.val + b.val, a.len + b.len}; 
}

void pre(int node = 1, int par = 1){
    in[node] = ++tim;
    up[node][0] = par;
    up_val[node][0] = dn_val[node][0] = val[node]; // Half-open interval

    for (int i = 1; i < LOG; i++) {
        up[node][i] = up[up[node][i-1]][i-1];
        
        // UP: path goes bottom-up. DOWN: path goes top-down.
        up_val[node][i] = combine(up_val[node][i-1], up_val[up[node][i-1]][i-1]);
        dn_val[node][i] = combine(dn_val[up[node][i-1]][i-1], dn_val[node][i-1]);
    }

    for (auto ch : gr[node]){
        if (ch == par) continue;
        pre(ch, node);
    }
    out[node] = ++tim;
}

bool if_anc(int x, int y){
    return in[x] <= in[y] && out[x] >= out[y];
}

int LCA(int x, int y){
    if (if_anc(x, y)) return x;
    if (if_anc(y, x)) return y;

    for (int i = LOG - 1; i >= 0; i--)
        if (!if_anc(up[x][i], y))
            x = up[x][i];

    return up[x][0];
}

Node query_path(int u, int v) {
    int lca = LCA(u, v);
    Node left_res, right_res;

    // 1. Jump u up to LCA
    for (int i = LOG - 1; i >= 0; i--) {
        if (!if_anc(up[u][i], lca)) {
            left_res = combine(left_res, up_val[u][i]);
            u = up[u][i];
        }
    }
    if (u != lca) left_res = combine(left_res, up_val[u][0]);

    // 2. Jump v up to LCA (Prepend to right_res!)
    for (int i = LOG - 1; i >= 0; i--) {
        if (!if_anc(up[v][i], lca)) {
            right_res = combine(dn_val[v][i], right_res); 
            v = up[v][i];
        }
    }
    if (v != lca) right_res = combine(dn_val[v][0], right_res);

    // 3. Merge: Left path -> LCA node -> Right path
    return combine(left_res, combine(val[lca], right_res));
}




// LCA , HASHING , BINARY SEARCH
#include<bits/stdc++.h>
using namespace std;
#define int long long
#define AhmedPlusPlus ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define hi cerr<<"HI\n";
#define el '\n'

/*                        ->    NO CLEAN CODE HERE    <-                        */

const int N = 1e5+5 , LOG = 18 , base = 31 , mod = 1e9+7;
int add(int x,int y){
    return (x + y + mod) % mod;
}
int mul(int a,int b){
    return (a * b)%mod;
}
int fp(int b, int p) {
    if (b == 1 or p == 0)
        return 1;

    int ret = fp(b, p >> 1);
    ret = mul(ret, ret);

    if (p & 1)
        ret = mul(ret, b);

    return ret;
}
int inv(int a) {
    return fp(a, mod-2);
}
vector<int>gr[N];
int n,q , up[N][LOG] , in[N] , out[N] , tim , dis[N]  , pw[N] , INV[N];
string s;

struct HA{
    int H = 0 , _H = 0 , sz = 0;
}HASH[N];

HA merge(HA a,HA b){
    HA rt;
    rt.H = mul(a.H , pw[b.sz]);
    rt.H = add(rt.H , b.H);

    rt._H = add(a._H , mul(b._H , pw[a.sz]));

    rt.sz = a.sz + b.sz;
    return rt;
}

HA single(char c){
    HA rt;
    rt.H = c - 'a' + 1;
    rt._H = c - 'a' + 1;
    rt.sz = 1;
    return rt;
}

HA sub(HA R , HA L){
    HA rt;
    int tar = R.sz - L .sz;
    // cout<<R.H<<" "<<L.H<<" "<<tar<<el;
    rt.H = add (R.H , - mul(L.H , pw[tar]));
    rt._H = add(R._H , -L._H);
    rt._H = mul(rt._H , INV[L.sz]);
    rt.sz = tar;
    return rt;
}

void init(){
    tim = 0 ;
    for (int i =1;i<=n;i++)gr[i].clear();
}

void pre(int node = 1 , int par = 1){
    in[node] = ++tim;
    up[node][0] = par;
    HASH[node] = single(s[node]);
    if(node!=1)
        HASH[node] = merge(HASH[par], HASH[node]);

    for (int i = 1 ; i < LOG ; i++)
        up[node][i] = up[up[node][i-1]][i-1];
    for (auto ch:gr[node])
        if (ch != par) {
            dis[ch] = dis[node] + 1;
            pre(ch, node);
        }
    out[node] = ++tim;
}

bool anc(int x,int y){
    return in[x] <= in[y] && out[x] >= out[y];
}

int LCA(int x,int y){
    if(anc(x,y))return x;
    if(anc(y,x))return y;
    for (int i = LOG - 1 ; i >= 0; --i)
        if (!anc(up[x][i] , y))
            x = up[x][i];
    return up[x][0];
}

int len(int x,int y){
    return dis[x] + dis[y] - dis[LCA(x,y)]*2;
}

int Kth(int node , int k){
    for(int i = LOG - 1 ; i >= 0 ; i--)
        if ((k >> i) & 1)
            node = up[node][i];
    return node;
}

int get_node(int from,int to,int tar){
    int lca = LCA(from,to);
    if (len(from, lca) >= tar)
        return Kth(from , tar);
    else
        return Kth(to, len(to,lca) - (tar - len(from, lca)) );
}

HA good (int from ,int to){
    // cout<<"test "<<from<<" "<<to<<el;
    return sub(HASH[from] , HASH[to]);
}

HA bad(int from,int to){
    HA rt = good(to,from);
    swap(rt.H , rt._H);
    return rt;
}

int get_hash(int from,int to){
    // cout<<from<<" "<<to<<el;
    int lca = LCA(from,to);
    // cout<<"lca "<<lca<<el;
    HA rt = bad(lca , from);
    // cout<<"test1 "<<rt.H<<el;
    rt = merge(rt , single(s[lca]));
    // cout<<"test2 "<<rt.H<<el;
    rt = merge(rt,good(to,lca));
    // cout<<"test3 "<<rt.H<<el;
    return rt.H;
}

int get(int from ,int to,int tar){
    // cout<<tar<<el;
    return get_hash(from , get_node(from , to , tar));
}

int solve(int a,int b,int x,int y){
    int sz1 = len(a , b);
    int sz2 = len(x , y);
    int st = 0 , ed = min(sz1 , sz2) , mid , fin = -1;
    while(st <= ed){
        mid = st + ed >> 1;
        if (get(a,b,mid) != get(x,y,mid))
            fin = mid , ed = mid - 1;
        else
            st = mid + 1;
    }
    if (fin == -1){
        if (sz1 > sz2)
            return 1;
        else if (sz1 < sz2)
            return 2;
        return 0;
    }
    else {
        char c1 = s[get_node(a,b,fin)];
        char c2 = s[get_node(x,y,fin)];
        if (c1 > c2)
            return 1;
        return 2;
    }
}

void pewpew(){

    cin>>n>>s;
    init();
    s = "*" + s;
    for (int i=0,x,y;i<n-1;i++){
        cin>>x>>y;
        gr[x].push_back(y);
        gr[y].push_back(x);
    }
    pre();
    cin>>q;
    while(q--){
        int a,b,x,y;cin>>a>>b>>x>>y;
        cout <<  solve(a,b,x,y) << '\n';
    }
    // cout << HASH[2].H << '\n';
    // cout << HASH[2]._H << '\n';

}

signed main() {
/* ^^^ */    AhmedPlusPlus    /* ^^^ */

//    ->> practice makes perfect


    pw[0] = INV[0] = 1;
    for (int i = 1; i<N;i++){
        pw[i] = mul(pw[i-1] , base);
        INV[i] = mul(INV[i-1] , inv(base));
    }
    int Hi;cin>>Hi;
    while(Hi--)
        pewpew();

}

//////////
// Kth ancestor

int get(int node,int tar){
    for (int i = LOG-1 ; i >= 0; i--)
        if ((1<<i) <= tar)
            node = up[node][i] , tar -= (1<<i);
    return node;
}







-----------------------------------------------------

  
