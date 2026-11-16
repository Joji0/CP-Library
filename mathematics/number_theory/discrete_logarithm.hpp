#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <unordered_map>
#include "mathematics/number_theory/number_theory.hpp"

// Solves a^x = b (mod m). Returns smallest non-negative integer x, or -1 if no solution.
// Works for arbitrary m (even when gcd(a, m) != 1) in O(sqrt(m)).
class DiscreteLogarithm {
       public:
	static long long solve(long long a, long long b, long long m) {
		assert(m > 0);
		a = NumberTheory::safe_modulo(a, m);
		b = NumberTheory::safe_modulo(b, m);
		if (m == 1) return 0;
		if (b == 1) return 0;
		if (a == 0) return b == 0 ? 1 : -1;

		long long k = 0, add = 1;
		long long g = std::gcd(a, m);
		while (g > 1) {
			if (b % g != 0) return -1;
			k++;
			b /= g;
			m /= g;
			add = (long long)((__int128_t)add * (a / g) % m);
			if (add == b) return k;
			g = std::gcd(a, m);
		}

		auto inv_add = NumberTheory::modular_inverse(add, m);
		if (!inv_add.has_value()) return -1;
		long long target = (long long)((__int128_t)b * inv_add.value() % m);

		long long M = (long long)std::ceil(std::sqrt((double)m));
		std::unordered_map<long long, long long> table;
		table.reserve(M);
		long long cur = 1;
		for (long long j = 0; j < M; ++j) {
			if (!table.count(cur)) {
				table[cur] = j;
			}
			cur = (long long)((__int128_t)cur * a % m);
		}

		long long a_M = cur;
		auto inv_a_M = NumberTheory::modular_inverse(a_M, m);
		if (!inv_a_M.has_value()) return -1;
		long long step_inv = inv_a_M.value();

		long long gamma = target;
		for (long long i = 0; i <= M; ++i) {
			auto it = table.find(gamma);
			if (it != table.end()) {
				long long ans = i * M + it->second + k;
				return ans;
			}
			gamma = (long long)((__int128_t)gamma * step_inv % m);
		}
		return -1;
	}
};
