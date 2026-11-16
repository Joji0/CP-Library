#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>
#include "mathematics/number_theory/number_theory.hpp"

class PrimitiveRoot {
       public:
	// Finds the smallest primitive root modulo prime p. Returns -1 if p is not prime or exceeds LLONG_MAX.
	static long long find(uint64_t p) {
		if (p == 2) return 1;
		if (p < 2 || p > uint64_t(std::numeric_limits<long long>::max()) || !NumberTheory::is_prime(p)) return -1;

		uint64_t phi = p - 1;
		auto primes = NumberTheory::factorize(phi);
		std::vector<uint64_t> distinct_factors;
		for (uint64_t q : primes) {
			if (distinct_factors.empty() || distinct_factors.back() != q) {
				distinct_factors.push_back(q);
			}
		}

		for (uint64_t g = 2; g < p; ++g) {
			bool ok = true;
			for (uint64_t q : distinct_factors) {
				if (NumberTheory::modular_power(g % p, phi / q, p) == 1) {
					ok = false;
					break;
				}
			}
			if (ok) return (long long)g;
		}
		return -1;
	}

	// Checks if g is a primitive root modulo prime p
	static bool is_primitive_root(uint64_t g, uint64_t p) {
		if (p == 2) return (g % 2 == 1);
		if (p < 2 || p > uint64_t(std::numeric_limits<long long>::max()) || g % p == 0 || !NumberTheory::is_prime(p)) return false;
		uint64_t phi = p - 1;
		auto primes = NumberTheory::factorize(phi);
		for (uint64_t q : primes) {
			if (NumberTheory::modular_power(g % p, phi / q, p) == 1) {
				return false;
			}
		}
		return true;
	}
};
