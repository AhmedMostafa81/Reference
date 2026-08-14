#include <bits/stdc++.h>
using namespace std;

// Segmented Sieve of Eratosthenes with Wheel Factorization (mod 30)
// Extremely fast for N up to 1e9.
vector<int> sieve(const int N, const int Q = 17, const int L = 1 << 15) {
    if (N < 2) return {};

    static const int rs[] = {1, 7, 11, 13, 17, 19, 23, 29};
    struct P {
        int p; 
        int pos[8];
        P(int p) : p(p), pos{0} {}
    };
    
    auto approx_prime_count = [](const int N) -> int {
        return N > 60184 ? N / (std::log(N) - 1.1)
                         : std::max(1.0, N / (std::log(N) - 1.11)) + 1;
    };

    const int v = std::sqrt(N), vv = std::sqrt(v);
    vector<bool> isp(v + 1, true);
    for (int i = 2; i <= vv; ++i) {
        if (isp[i]) {
            for (int j = i * i; j <= v; j += i) isp[j] = false;
        }
    }

    const int rsize = approx_prime_count(N + 30);
    vector<int> primes;
    primes.reserve(rsize);
    primes.push_back(2);
    primes.push_back(3);
    primes.push_back(5);

    vector<P> sprimes; 
    size_t pbeg = 0;
    int prod = 1;
    
    for (int p = 7; p <= v; ++p) {
        if (!isp[p]) continue;
        if (p <= Q) {
            prod *= p;
            ++pbeg;
            primes.push_back(p);
        }
        auto pp = P(p);
        for (int t = 0; t < 8; ++t) {
            int j = (p <= Q) ? p : p * p;
            while (j % 30 != rs[t]) j += p << 1;
            pp.pos[t] = j / 30;
        }
        sprimes.push_back(pp);
    }

    vector<unsigned char> pre(prod, 0xFF);
    for (size_t pi = 0; pi < pbeg; ++pi) {
        auto pp = sprimes[pi]; 
        const int p = pp.p;
        for (int t = 0; t < 8; ++t) {
            const unsigned char m = ~(1 << t);
            for (int i = pp.pos[t]; i < prod; i += p) {
                pre[i] &= m;
            }
        }
    }

    const int block_size = (L + prod - 1) / prod * prod;
    vector<unsigned char> block(block_size);
    const int M = (N + 29) / 30;

    for (int beg = 0; beg < M; beg += block_size) {
        int end = std::min(M, beg + block_size);
        int local_end = end - beg;

        // Tile the pre-computed block
        for (int i = 0; i < local_end; i += prod) {
            std::copy(pre.begin(), pre.end(), block.begin() + i);
        }
        
        if (beg == 0) block[0] &= 0xFE; // 1 is not prime

        // Sieve the current segment
        for (size_t pi = pbeg; pi < sprimes.size(); ++pi) {
            auto& pp = sprimes[pi];
            const int p = pp.p;
            for (int t = 0; t < 8; ++t) {
                int i = pp.pos[t]; 
                const unsigned char m = ~(1 << t);
                int local_i = i - beg;
                for (; local_i < local_end; local_i += p) {
                    block[local_i] &= m;
                }
                pp.pos[t] = local_i + beg; // Save state for next block
            }
        }

        // Extract primes
        for (int local_i = 0; local_i < local_end; ++local_i) {
            int i = beg + local_i;
            for (int m = block[local_i]; m > 0; m &= m - 1) {
                primes.push_back(i * 30 + rs[__builtin_ctz(m)]);
            }
        }
    }

    // Safely trim any generated primes that overshoot N
    while (!primes.empty() && primes.back() > N) {
        primes.pop_back();
    }
    
    return primes;
}

// Example usage
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Get all primes up to 100,000,000
    vector<int> p = sieve(100000000);
    cout << "Count: " << p.size() << "\n";
    if (!p.empty()) cout << "Last prime: " << p.back() << "\n";

    return 0;
}
