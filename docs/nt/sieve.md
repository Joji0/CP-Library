---
title: Sieve (Linear)
documentation_of: nt/sieve.hpp
---

# Sieve (Linear)

## Overview

A **linear sieve** that computes number-theoretic functions for all integers up to $N$ in $O(N)$ time. Precomputes:

| Array | Description |
|-------|-------------|
| `primes` | All primes $\le N$ |
| `spf[i]` | Smallest prime factor of $i$ |
| `phi[i]` | [Euler's totient](https://en.wikipedia.org/wiki/Euler%27s_totient_function) $\varphi(i)$ |
| `mu[i]` | [Möbius function](https://en.wikipedia.org/wiki/M%C3%B6bius_function) $\mu(i)$ |

Additionally provides utility functions for factorization and divisor enumeration, all leveraging the precomputed `spf` array for $O(\log n)$ factorization.

**Namespace**: `NT`

## Constructor

### `NT::Sieve(int max_n)`

Runs the linear sieve for all integers in $[1, \mathrm{max\_n}]$.

**Constraints**: $\mathrm{max\_n} \ge 1$

**Complexity**: $O(\mathrm{max\_n})$ time and space

## Methods

### `bool is_prime(int x)`

Returns `true` if $x$ is prime.

**Constraints**: $1 \le x \le N$

**Complexity**: $O(1)$

### `std::vector<std::pair<int, int>> get_prime_factors(int x)`

Returns the prime factorization of $x$ as a vector of $(p, e)$ pairs, meaning $x = \prod p^e$.

**Constraints**: $1 \le x \le N$

**Complexity**: $O(\log x)$

### `std::vector<int> get_distinct_primes(int x)`

Returns the distinct prime factors of $x$ (without exponents).

**Constraints**: $1 \le x \le N$

**Complexity**: $O(\log x)$

### `int64_t count_divisors(int x)`

Returns $\sigma_0(x) = \sum_{d | x} 1$, the number of positive divisors of $x$.

**Constraints**: $1 \le x \le N$

**Complexity**: $O(\log x)$

### `int64_t sum_divisors(int x)`

Returns $\sigma_1(x) = \sum_{d | x} d$, the sum of positive divisors of $x$.

**Constraints**: $1 \le x \le N$

**Complexity**: $O(\log x)$

### `std::vector<int64_t> get_all_divisors(int x)`

Returns all positive divisors of $x$ (not necessarily sorted).

**Constraints**: $1 \le x \le N$

**Complexity**: $O(\sigma_0(x))$ — proportional to the number of divisors

## Members

| Member | Description |
|--------|-------------|
| `N` | The sieve limit |
| `primes` | `std::vector<int>` of all primes $\le N$ |
| `spf[i]` | Smallest prime factor of $i$ |
| `phi[i]` | Euler's totient $\varphi(i)$ |
| `mu[i]` | Möbius function $\mu(i)$: $1$ if squarefree with even number of prime factors, $-1$ if odd, $0$ if not squarefree |

## Usage Example

```cpp
#include "nt/sieve.hpp"

int main() {
    NT::Sieve sv(1000000);

    sv.is_prime(97);            // true
    sv.is_prime(100);           // false
    sv.primes.size();           // 78498 (number of primes ≤ 10^6)

    sv.phi[12];                 // 4
    sv.mu[6];                   // 1 (6 = 2·3, squarefree, 2 prime factors)
    sv.mu[12];                  // 0 (12 = 2²·3, not squarefree)

    auto factors = sv.get_prime_factors(360);
    // [(2,3), (3,2), (5,1)] → 360 = 2³·3²·5

    sv.count_divisors(360);     // 24
    sv.sum_divisors(360);       // 1170

    auto divs = sv.get_all_divisors(12);
    // {1, 2, 4, 3, 6, 12}
}
```

## Source Code

```cpp
#pragma once
#include <cstdint>
#include <vector>

namespace NT {
struct Sieve {
        int N;
        std::vector<int> primes;
        std::vector<int> spf;
        std::vector<int> phi;
        std::vector<int> mu;
        Sieve(int max_n) : N(max_n), spf(max_n + 1, 0), phi(max_n + 1), mu(max_n + 1) {
                phi[1] = 1;
                mu[1] = 1;
                for (int i = 2; i <= N; i++) {
                        if (spf[i] == 0) {
                                spf[i] = i;
                                primes.push_back(i);
                                phi[i] = i - 1;
                                mu[i] = -1;
                        }
                        for (int p : primes) {
                                if (p > spf[i] || (int64_t)i * p > N) break;
                                spf[i * p] = p;
                                if (spf[i] == p) {
                                        phi[i * p] = phi[i] * p;
                                        mu[i * p] = 0;
                                        break;
                                } else {
                                        phi[i * p] = phi[i] * (p - 1);
                                        mu[i * p] = -mu[i];
                                }
                        }
                }
        }
        bool is_prime(int x) const {
                if (x <= 1 || x > N) return false;
                return spf[x] == x;
        }
        std::vector<std::pair<int, int>> get_prime_factors(int x) const {
                std::vector<std::pair<int, int>> factors;
                while (x > 1) {
                        int p = spf[x];
                        int exponent = 0;
                        while (x % p == 0) {
                                x /= p;
                                exponent++;
                        }
                        factors.push_back({p, exponent});
                }
                return factors;
        }
        std::vector<int> get_distinct_primes(int x) const {
                std::vector<int> distinct;
                while (x > 1) {
                        int p = spf[x];
                        distinct.push_back(p);
                        while (x % p == 0) x /= p;
                }
                return distinct;
        }
        int64_t count_divisors(int x) const {
                if (x == 1) return 1;
                int64_t res = 1;
                while (x > 1) {
                        int p = spf[x];
                        int e = 0;
                        while (x % p == 0) {
                                x /= p;
                                e++;
                        }
                        res *= (e + 1);
                }
                return res;
        }
        int64_t sum_divisors(int x) const {
                if (x == 1) return 1;
                int64_t res = 1;
                while (x > 1) {
                        int p = spf[x];
                        int64_t sum_p = 1, p_pow = 1;
                        while (x % p == 0) {
                                x /= p;
                                p_pow *= p;
                                sum_p += p_pow;
                        }
                        res *= sum_p;
                }
                return res;
        }
        std::vector<int64_t> get_all_divisors(int x) const {
                auto factors = get_prime_factors(x);
                std::vector<int64_t> divs = {1};
                for (auto &pf : factors) {
                        int p = pf.first;
                        int count = pf.second;
                        int sz = divs.size();
                        int64_t cur_p = 1;
                        for (int i = 0; i < count; ++i) {
                                cur_p *= p;
                                for (int j = 0; j < sz; ++j) {
                                        divs.push_back(divs[j] * cur_p);
                                }
                        }
                }
                return divs;
        }
};
} // namespace NT

```
