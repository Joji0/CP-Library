---
title: Sieve (Linear)
documentation_of: nt/sieve.hpp
---

Linear sieve. Builds prime list and smallest prime factor table in $O(n)$.

### Usage

- `Sieve(int n)`: build sieve for $[0, n]$
- `is_prime(x)`: whether $x$ is prime
- `get_prime_factors(x)`: prime factorization $\{(p, e)\}$
- `get_distinct_primes(x)`: list of distinct prime factors
- `count_divisors(x)`: number of divisors
- `sum_divisors(x)`: sum of divisors
- `get_all_divisors(x)`: list of all divisors
