void mobius()
{
    mob[1] = 1;
    for (int x = 1; x < MX; x++)
        for (int y = 2 * x; y < MX; y += x)
            mob[y] -= mob[x];
}

==============================================

  #include <iostream>
#include <vector>

using namespace std;

// Precomputes mu[1..N] in O(N) time and space
struct MobiusSieve {
    int n;
    vector<int> mu;
    vector<int> primes;
    vector<bool> is_prime;

    MobiusSieve(int N) : n(N), mu(N + 1, 0), is_prime(N + 1, true) {
        sieve();
    }

    void sieve() {
        if (n < 1) return;
        mu[1] = 1;
        is_prime[0] = is_prime[1] = false;

        for (int i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                mu[i] = -1; // Prime has 1 prime factor -> (-1)^1 = -1
            }
            for (int p : primes) {
                if (i * p > n) break;
                is_prime[i * p] = false;
                
                if (i % p == 0) {
                    // i * p contains p^2, so it's not square-free
                    mu[i * p] = 0; 
                    break;
                } else {
                    // i * p gets one additional prime factor -> flip sign
                    mu[i * p] = -mu[i]; 
                }
            }
        }
    }
};
