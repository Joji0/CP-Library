#pragma once
#include <cassert>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>
#include <utility>

class Sieve {
	int n;
	std::vector<int> min_prime;
	std::vector<int> prime_list;
	std::vector<int> phi_table;
	std::vector<int> mu_table;

       public:
	Sieve(int max_n = 0) { initialize(max_n); }
	void initialize(int max_n) {
		if (max_n < 0 || max_n == std::numeric_limits<int>::max()) throw std::invalid_argument("sieve size outside supported interval");
		n = max_n;
		min_prime.assign(n + 1, 0);
		phi_table.assign(n + 1, 0);
		mu_table.assign(n + 1, 0);
		prime_list.clear();
		if (n <= 0) return;
		phi_table[1] = 1;
		mu_table[1] = 1;
		for (int i = 2; i <= n; ++i) {
			if (min_prime[i] == 0) {
				min_prime[i] = i;
				prime_list.push_back(i);
				phi_table[i] = i - 1;
				mu_table[i] = -1;
			}
			for (int p : prime_list) {
				if (p > min_prime[i] || (long long)i * p > n) break;
				min_prime[i * p] = p;
				if (min_prime[i] == p) {
					phi_table[i * p] = phi_table[i] * p;
					mu_table[i * p] = 0;
					break;
				} else {
					phi_table[i * p] = phi_table[i] * (p - 1);
					mu_table[i * p] = -mu_table[i];
				}
			}
		}
	}
	int size() const { return n; }
	bool is_prime(int x) const {
		if (x <= 1 || x > n) return false;
		return min_prime[x] == x;
	}
	int smallest_prime_factor(int x) const {
		assert(1 <= x && x <= n);
		return min_prime[x];
	}
	const std::vector<int>& primes() const {
		return prime_list;
	}
	int totient(int x) const {
		assert(1 <= x && x <= n);
		return phi_table[x];
	}
	int mobius(int x) const {
		assert(1 <= x && x <= n);
		return mu_table[x];
	}
	std::vector<std::pair<int, int>> factorize(int x) const {
		assert(1 <= x && x <= n);
		std::vector<std::pair<int, int>> factors;
		while (x > 1) {
			int p = min_prime[x];
			int exponent = 0;
			while (x % p == 0) {
				x /= p;
				exponent++;
			}
			factors.emplace_back(p, exponent);
		}
		return factors;
	}
	std::vector<int> distinct_primes(int x) const {
		assert(1 <= x && x <= n);
		std::vector<int> distinct;
		while (x > 1) {
			int p = min_prime[x];
			distinct.push_back(p);
			while (x % p == 0) x /= p;
		}
		return distinct;
	}
	std::vector<int> divisors(int x) const {
		assert(1 <= x && x <= n);
		std::vector<int> res = {1};
		auto factors = factorize(x);
		for (const auto& [p, e] : factors) {
			int sz = (int)res.size();
			int p_pow = 1;
			for (int i = 1; i <= e; ++i) {
				p_pow *= p;
				for (int j = 0; j < sz; ++j) {
					res.push_back(res[j] * p_pow);
				}
			}
		}
		return res;
	}
	int count_divisors(int x) const {
		assert(1 <= x && x <= n);
		int res = 1;
		while (x > 1) {
			int p = min_prime[x];
			int e = 0;
			while (x % p == 0) {
				x /= p;
				e++;
			}
			res *= (e + 1);
		}
		return res;
	}
	long long sum_divisors(int x) const {
		assert(1 <= x && x <= n);
		long long res = 1;
		while (x > 1) {
			int p = min_prime[x];
			long long cur_sum = 1, p_pow = 1;
			while (x % p == 0) {
				x /= p;
				p_pow *= p;
				cur_sum += p_pow;
			}
			res *= cur_sum;
		}
		return res;
	}
};
