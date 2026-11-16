#pragma once
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <limits>
#include <numeric>
#include <optional>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

class NumberTheory {
       public:
	static inline long long safe_modulo(long long x, long long m) {
		if (m <= 0) throw std::invalid_argument("modulus must be positive");
		x %= m;
		if (x < 0) x += m;
		return x;
	}

	static inline long long modular_power(long long a, uint64_t n, long long mod) {
		if (mod <= 0) throw std::invalid_argument("modulus must be positive");
		long long res = 1 % mod;
		a = safe_modulo(a, mod);
		while (n > 0) {
			if (n & 1) res = (long long)((__int128_t)res * a % mod);
			a = (long long)((__int128_t)a * a % mod);
			n >>= 1;
		}
		return res;
	}

	// Returns nullopt only when the nonnegative gcd does not fit in long long.
	// On success a*x + b*y = gcd(a,b), evaluated in a sufficiently wide type.
	static std::optional<long long> try_extended_greatest_common_divisor(long long a, long long b, long long &x, long long &y) {
		__int128 first = a, second = b, first_x = 1, second_x = 0, first_y = 0, second_y = 1;
		while (second != 0) {
			const __int128 quotient = first / second;
			const __int128 remainder = first % second;
			first = second;
			second = remainder;
			const __int128 next_x = first_x - quotient * second_x;
			const __int128 next_y = first_y - quotient * second_y;
			first_x = second_x;
			second_x = next_x;
			first_y = second_y;
			second_y = next_y;
		}
		if (first < 0) {
			first = -first;
			first_x = -first_x;
			first_y = -first_y;
		}
		if (first > std::numeric_limits<long long>::max()) return std::nullopt;
		x = static_cast<long long>(first_x);
		y = static_cast<long long>(first_y);
		return static_cast<long long>(first);
	}

	static long long extended_greatest_common_divisor(long long a, long long b, long long &x, long long &y) {
		auto result = try_extended_greatest_common_divisor(a, b, x, y);
		if (!result) throw std::overflow_error("greatest common divisor exceeds long long");
		return *result;
	}

	// Returns nullopt for a nonunit or modulus <= 1.
	static std::optional<long long> modular_inverse(long long a, long long mod) {
		if (mod <= 1) return std::nullopt;
		long long x, y;
		if (extended_greatest_common_divisor(safe_modulo(a, mod), mod, x, y) != 1) return std::nullopt;
		return safe_modulo(x, mod);
	}

	// Generates the inverse table for 1..N in O(N), with prime mod and N < mod.
	static inline std::vector<long long> inverse_table(int n, long long mod) {
		assert(0 <= n && n < mod);
		std::vector<long long> inverse(std::size_t(n) + 1);
		if (n >= 1) inverse[1] = 1;
		for (long long i = 2; i <= n; ++i) {
			inverse[i] = static_cast<long long>(__int128(mod - mod / i) * inverse[mod % i] % mod);
		}
		return inverse;
	}

	// Finds x = r[i] (mod m[i]); inputs use a dummy at index 0.
	// Returns {smallest nonnegative solution, combined modulus}, or {0,0}
	// for inconsistent congruences. Throws on nonrepresentable combined modulus.
	static inline std::pair<long long, long long> chinese_remainder_theorem(const std::vector<long long> &r, const std::vector<long long> &m) {
		if (r.empty() || r.size() != m.size()) throw std::invalid_argument("expected congruence vectors with a dummy slot");
		long long r0 = 0, m0 = 1;
		for (std::size_t i = 1; i < r.size(); ++i) {
			if (m[i] <= 0) throw std::invalid_argument("modulus must be positive");
			long long r1 = safe_modulo(r[i], m[i]), m1 = m[i];
			if (m0 < m1) {
				std::swap(r0, r1);
				std::swap(m0, m1);
			}
			if (m0 % m1 == 0) {
				if (r0 % m1 != r1) return {0, 0};
				continue;
			}
			long long x, y;
			long long g = extended_greatest_common_divisor(m0, m1, x, y);
			if ((r1 - r0) % g != 0) return {0, 0};
			long long u1 = m1 / g;
			long long t = (long long)(((__int128_t)r1 - r0) / g * x % u1);
			__int128_t next_mod = (__int128_t)m0 * u1;
			if (next_mod > std::numeric_limits<long long>::max()) throw std::overflow_error("combined modulus exceeds long long");
			__int128_t next_r = (__int128_t)r0 + (__int128_t)t * m0;
			m0 = (long long)next_mod;
			r0 = (long long)(next_r % m0);
			r0 = safe_modulo(r0, m0);
		}
		return {r0, m0};
	}

	// Sum floor((a*i+b)/m) for 0 <= i < n. Requires n,a,b >= 0 and m > 0.
	// All contributions are nonnegative. Bound before multiplying to avoid even 128-bit overflow.
	static std::optional<long long> try_floor_sum(long long n, long long m, long long a, long long b) {
		if (n < 0 || m <= 0 || a < 0 || b < 0) throw std::invalid_argument("expected nonnegative floor-sum parameters and positive modulus");
		long long answer = 0;
		auto accumulate = [&](const __int128 count, const long long value) {
			if (value == 0) return true;
			if (count > (std::numeric_limits<long long>::max() - answer) / value) return false;
			answer += static_cast<long long>(count * value);
			return true;
		};
		while (true) {
			if (!accumulate(__int128(n) * (n - 1) / 2, a / m) || !accumulate(n, b / m)) return std::nullopt;
			a %= m;
			b %= m;
			const __int128 next = __int128(a) * n + b;
			if (next < m) return answer;
			n = static_cast<long long>(next / m);
			b = static_cast<long long>(next % m);
			std::swap(m, a);
		}
	}

	static long long floor_sum(long long n, long long m, long long a, long long b) {
		auto result = try_floor_sum(n, m, a, b);
		if (!result) throw std::overflow_error("floor sum exceeds long long");
		return *result;
	}

	// 64-bit deterministic Miller-Rabin primality test
	static inline bool is_prime(uint64_t n) {
		if (n < 2) return false;
		for (uint64_t p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
			if (n % p == 0) return n == p;
		}
		uint64_t d = n - 1, s = 0;
		while ((d & 1) == 0) {
			d >>= 1;
			s++;
		}
		auto mod_mul = [](uint64_t a, uint64_t b, uint64_t mod) -> uint64_t {
			return (uint64_t)((__uint128_t)a * b % mod);
		};
		auto mod_pow_u = [&](uint64_t base, uint64_t exp, uint64_t mod) -> uint64_t {
			uint64_t res = 1;
			base %= mod;
			while (exp > 0) {
				if (exp & 1) res = mod_mul(res, base, mod);
				base = mod_mul(base, base, mod);
				exp >>= 1;
			}
			return res;
		};
		for (uint64_t a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
			if (a % n == 0) continue;
			uint64_t x = mod_pow_u(a, d, n);
			if (x == 1 || x == n - 1) continue;
			bool composite = true;
			for (uint64_t r = 1; r < s; r++) {
				x = mod_mul(x, x, n);
				if (x == n - 1) {
					composite = false;
					break;
				}
			}
			if (composite) return false;
		}
		return true;
	}

	// 64-bit Pollard's Rho algorithm: finds a non-trivial factor of composite n in O(n^(1/4))
	static inline uint64_t pollard_rho(uint64_t n) {
		assert(n > 1 && !is_prime(n));
		if (n % 2 == 0) return 2;
		static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
		while (true) {
			uint64_t c = std::uniform_int_distribution<uint64_t>(1, n - 1)(rng);
			uint64_t x = std::uniform_int_distribution<uint64_t>(0, n - 1)(rng);
			uint64_t y = x, d = 1;
			auto f = [&](uint64_t v) { return (uint64_t)(((__uint128_t)v * v + c) % n); };
			while (d == 1) {
				x = f(x);
				y = f(f(y));
				uint64_t diff = x > y ? x - y : y - x;
				d = std::gcd(diff, n);
			}
			if (d != n) return d;
		}
	}

	// Fast 64-bit prime factorization up to 10^18 -> returns sorted prime factors
	static inline std::vector<uint64_t> factorize(uint64_t n) {
		if (n <= 1) return {};
		std::vector<uint64_t> res;
		auto factor_rec = [&](auto &self, uint64_t val) -> void {
			if (val == 1) return;
			if (is_prime(val)) {
				res.push_back(val);
				return;
			}
			uint64_t d = pollard_rho(val);
			self(self, d);
			self(self, val / d);
		};
		factor_rec(factor_rec, n);
		std::sort(res.begin(), res.end());
		return res;
	}

	// Returns all positive divisors of n (n <= 10^18) in sorted order
	static inline std::vector<uint64_t> divisors(uint64_t n) {
		if (n == 0) return {};
		auto primes = factorize(n);
		std::vector<std::pair<uint64_t, int>> factors;
		for (uint64_t p : primes) {
			if (factors.empty() || factors.back().first != p) {
				factors.emplace_back(p, 1);
			} else {
				factors.back().second++;
			}
		}
		std::vector<uint64_t> res = {1};
		for (const auto &[p, e] : factors) {
			int sz = (int)res.size();
			uint64_t p_pow = 1;
			for (int i = 1; i <= e; ++i) {
				p_pow *= p;
				for (int j = 0; j < sz; ++j) {
					res.push_back(res[j] * p_pow);
				}
			}
		}
		std::sort(res.begin(), res.end());
		return res;
	}
};
