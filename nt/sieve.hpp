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
