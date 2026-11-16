#pragma once
#include <cassert>
#include <cstdint>
#include "mathematics/number_theory/number_theory.hpp"

// Tonelli-Shanks algorithm: finds x such that x^2 = a (mod p) for prime p.
// Returns x in [0, p-1] if a solution exists, or -1 if no solution exists.
class ModularSquareRoot {
       public:
	static long long solve(long long a, long long p) {
		assert(p >= 2);
		a = NumberTheory::safe_modulo(a, p);
		if (a == 0) return 0;
		if (p == 2) return a;
		if (NumberTheory::modular_power(a, (p - 1) / 2, p) != 1) {
			return -1;  // Not a quadratic residue
		}
		if (p % 4 == 3) {
			return NumberTheory::modular_power(a, p / 4 + 1, p);
		}

		long long q = p - 1;
		long long s = 0;
		while ((q & 1) == 0) {
			q >>= 1;
			s++;
		}

		long long z = 2;
		while (NumberTheory::modular_power(z, (p - 1) / 2, p) != p - 1) {
			z++;
		}

		long long m = s;
		long long c = NumberTheory::modular_power(z, q, p);
		long long t = NumberTheory::modular_power(a, q, p);
		long long r = NumberTheory::modular_power(a, (q + 1) / 2, p);

		while (t != 1) {
			long long i = 0;
			long long cur = t;
			while (cur != 1) {
				cur = (long long)((__int128_t)cur * cur % p);
				i++;
				if (i == m) return -1;
			}
			long long b = c;
			for (long long j = 0; j < m - i - 1; ++j) {
				b = (long long)((__int128_t)b * b % p);
			}
			m = i;
			c = (long long)((__int128_t)b * b % p);
			t = (long long)((__int128_t)t * c % p);
			r = (long long)((__int128_t)r * b % p);
		}
		return r;
	}
};
