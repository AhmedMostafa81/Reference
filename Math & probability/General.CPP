/*
quadratic equation
a * x^2 + b * x + c = 0
x = (-b (+or-) sqrtl(b * b - 4 * a * c) ) / (2 * a)


to get sum 2^(0*n) + 2^(1*n) + 2^(2*n) + 2^(3*n) + 2^(4*n) + .... + 2^(k*n)

 the formula is
    (   (2 ^ ((k+1)*n)) - 1   ) /   (  2^n - 1   )



  to get	a^1 + a^2 + a^3 + .... + a^k
    the formula is (a * (a^k - 1)) / (a-1)
    this doesn't work for a = 1
*/

double LOG( int x, int k){
    double answer;
    answer = log10( x ) / log10( k );
    return answer;
}

const int N = 2e5+7, P1 = 31, P2 = 37, mod= 1e9 + 7;

bitset<N> notPrime;
void sieve() {
    notPrime[0] = notPrime[1] = 1;
    for(int i = 2; i*i < N; i++) {
        if(notPrime[i]) continue;

        for(int j = i*i; j < N; j+=i)
            notPrime[j] = 1;
    }
}

int spf[N];
void SPF() {
    for(int i = 1; i < N; i++)
        spf[i] = i;
    for(int i = 2; i*i < N; i++) {
        if(spf[i] != i) continue;

        for(int j = i*i; j < N; j+=i)
            spf[j] = min(spf[j], i);
    }
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
    return fastPower(a, mod-2);
}

int divi(int a, int b) {
    return mult(a, inv(b));
}
 
int fact[N];

int nPr(int n, int r) {
    if(r > n) return 0;
    if(n < 0 || r < 0) return 0;
    return divi(fact[n], fact[n-r]);
}

int nCr(int n, int r) {
    if(r > n) return 0;
    if(n < 0 || r < 0) return 0;
    return divi(fact[n], mult(fact[r], fact[n-r]));
}

int sNb(int stars ,int bars) {
    return nCr(stars + bars , min(stars , (stars + bars) - stars));
}
-------------------------------------------------
// 2. Precompute factorials and their modular inverses
void precompute() {
    fact[0] = 1;
    invFact[0] = 1;
    
    // Calculate all factorials normally
    for (int i = 1; i < MAXN; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    
    // Fermat's Little Theorem for the inverse of the maximum factorial
    invFact[MAXN - 1] = power(fact[MAXN - 1], MOD - 2);
    
    // Calculate the rest backwards in O(N)
    for (int i = MAXN - 2; i >= 1; i--) {
        invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
    }
}
--------------------------------------------------
long long c(int n, int k) {
     long long result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

long long nCr(long long n, long long k) {
    k = min(k, n - k);
    __int128 res = 1;

    for (long long i = 1; i <= k; i++)
        res = res * (n - k + i) / i;

    return (long long)res;
}

ll p(int n, int k) {
    ll result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
    }
    return result;
}
