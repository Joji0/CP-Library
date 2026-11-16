#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>
#include "mathematics/number_theory/number_theory.hpp"

// Computes binomial coefficients mod p^e with no division by nonunits.
// If p^e <= table_limit, preprocessing is O(p^e), queries O(log_p n).
// Otherwise use O(min(r, n-r) log_p n) time and O(1) auxiliary memory.
class PrimePowerBinomialCoefficient {
	long long prime, modulus = 1;
	int exponent;
	std::vector<long long> prefix, inverse_prefix;

	long long multiply(long long a, long long b) const { return __int128(a) * b % modulus; }

	long long valuation(uint64_t n) const {
		uint64_t result = 0;
		while (n != 0) result += (n /= prime);
		return static_cast<long long>(result);
	}
	long long unit_factorial(uint64_t n, const std::vector<long long>& table) const {
		long long result = 1;
		for (; n != 0; n /= prime) {
			result = multiply(result, table[n % modulus]);
			// The product of all units is 1 or -1, and equals its own inverse.
			if (((n / modulus) & 1) && prefix.back() != 1) result = modulus - result;
		}
		return result;
	}

       public:
	PrimePowerBinomialCoefficient(long long prime, int exponent, long long table_limit = 2'000'000)
	    : prime(prime), exponent(exponent) {
		if (exponent < 1 || prime < 2 || !NumberTheory::is_prime(prime)) throw std::invalid_argument("invalid prime power");
		for (int i = 0; i < exponent; ++i) {
			if (modulus > std::numeric_limits<long long>::max() / prime) throw std::overflow_error("prime power exceeds long long");
			modulus *= prime;
		}
		if (modulus > table_limit) return;
		prefix.assign(modulus, 1);
		inverse_prefix.assign(modulus, 1);
		for (long long i = 1; i < modulus; ++i) {
			prefix[i] = (i % prime == 0) ? prefix[i - 1] : multiply(prefix[i - 1], i);
		}
		inverse_prefix.back() = prefix.back();
		for (long long i = modulus - 1; i > 0; --i) {
			inverse_prefix[i - 1] = (i % prime == 0) ? inverse_prefix[i] : multiply(inverse_prefix[i], i);
		}
	}
	long long get_modulus() const { return modulus; }
	long long binomial_coefficient(long long n, long long r) const {
		if (r < 0 || r > n) return 0;
		r = std::min(r, n - r);
		if (r == 0) return 1;
		long long power = valuation(n) - valuation(r) - valuation(n - r);
		if (power >= exponent) return 0;

		long long unit = 1;
		if (!prefix.empty()) {
			unit = multiply(unit_factorial(n, prefix), unit_factorial(r, inverse_prefix));
			unit = multiply(unit, unit_factorial(n - r, inverse_prefix));
		} else {
			long long denominator = 1;
			for (long long i = 1; i <= r; ++i) {
				long long a = n - r + i, b = i;
				while (a % prime == 0) a /= prime;
				while (b % prime == 0) b /= prime;
				unit = multiply(unit, a % modulus);
				denominator = multiply(denominator, b % modulus);
			}
			unit = multiply(unit, *NumberTheory::modular_inverse(denominator, modulus));
		}
		return multiply(unit, NumberTheory::modular_power(prime, power, modulus));
	}
};
