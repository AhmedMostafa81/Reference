The geometric mean of the positive divisors of X equals sqrt(X)

geometric mean of a1 , a2 , a3 , a4
is 4th_root(a1*a2*a3*a4)



int mul_mod(int a,int b , int mod) {
    return ((__int128)a * b) % mod; // int128 is important here
}

int pow_mod(int x , int pw , int mod) {
    if (pw == 0)
        return 1;
    int rt = pow_mod(mul_mod(x , x , mod) , pw >> 1 , mod);
    if (pw & 1)
        rt = mul_mod(rt , x , mod);
    return rt;
}

/* ----------- Miller-Rabin ----------- */
// log or log^2
 
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23}) {
        if (n == p) return true;
        if (n % p == 0) return n == p;
    }
 
    ll d = n - 1, s = 0;
    while ((d & 1) == 0) d >>= 1, s++;
 
    for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (a % n == 0) continue;
        ll x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                comp = false;
                break;
            }
        }
        if (comp) return false;
    }
    return true;
}

------------------------------------------------------------------------------------------

/* ----------- Pollard Rho ----------- */
 // find any divisor 1 < d < n
 // time -> n^(1/4)
 
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
 
ll Pollard(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
 
    ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
    ll x = uniform_int_distribution<ll>(0, n - 1)(rng);
    ll y = x, d = 1;
 
    auto f = [&](ll x) {
        return (mul_mod(x, x, n) + c) % n;
    };
 
    while (d == 1) {
        x = f(x);
        y = f(f(y));
        d = gcd(abs(x - y), n);
    }
 
    return d == n ? Pollard(n) : d;
}

// ===========================================
// fast prime factorization
factor(n):
    if prime -> done
    d = Pollard(n)
    factor(d)
    factor(n/d)
/*
For prime factorization with that recursive scheme, the usual answer is:

Expected time ≈ O(n^(1/4))

for a 64-bit number, with some log n factors from modular arithmetic and primality checks.

So a more careful way to say it is:

O(n^(1/4) * log n) or O(n ^ (1/4) * log(n)^2)

depending on how mul_mod / pow_mod are implemented.
 */
