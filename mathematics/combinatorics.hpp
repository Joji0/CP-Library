#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include "mathematics/number_theory/number_theory.hpp"

// Prime modulus. Factorial caches grow only for small indices. Use the
// prime-power/composite variants when the modulus is not prime.
template <typename Modular>
class Combinatorics {
	static constexpr int cache_size_limit = 1 << 22;
	mutable long long modulus = 0;
	mutable std::vector<Modular> factorials, inverse_factorials;

	void synchronize() const {
		const long long current = Modular::get_modulus();
		if (modulus == current) return;
		if (!NumberTheory::is_prime(current)) throw std::invalid_argument("Combinatorics requires a prime modulus");
		modulus = current;
		factorials.assign(1, Modular(1));
		inverse_factorials.assign(1, Modular(1));
	}

	void extend(int n) const {
		const int first = int(factorials.size());
		if (n < first) return;
		const int size = int(std::min<long long>(modulus, std::max(n + 1LL, 2LL * first)));
		factorials.resize(size);
		inverse_factorials.resize(size);
		for (int i = first; i < size; ++i) factorials[i] = factorials[i - 1] * Modular(i);
		inverse_factorials.back() = factorials.back().inverse();
		for (int i = size - 1; i > first; --i) inverse_factorials[i - 1] = inverse_factorials[i] * Modular(i);
	}

	Modular digit_binomial(long long n, long long r) const {
		if (r > n) return 0;
		r = std::min(r, n - r);
		if (r == 0) return 1;
		if (n < cache_size_limit || n < static_cast<long long>(factorials.size())) {
			extend(int(n));
			return factorials[n] * inverse_factorials[r] * inverse_factorials[n - r];
		}
		Modular numerator = 1, denominator = 1;
		for (long long i = 1; i <= r; ++i) {
			numerator *= Modular(n - r + i);
			denominator *= Modular(i);
		}
		return numerator / denominator;
	}

	Modular lucas(uint64_t n, uint64_t r) const {
		Modular result = 1;
		while (r != 0) {
			if (r % modulus > n % modulus) return 0;
			result *= digit_binomial(n % modulus, r % modulus);
			n /= modulus;
			r /= modulus;
		}
		return result;
	}

       public:
	explicit Combinatorics(int maximum_hint = 2) { reserve(maximum_hint); }

	// Request O(n) preprocessing for O(1) factorial and digit-binomial queries.
	// Tied to the current modulus; a change clears the cache on the next call.
	void reserve(int n) const {
		synchronize();
		if (n >= 0) extend(int(std::min<long long>(n, modulus - 1)));
	}

	Modular factorial(int n) const {
		synchronize();
		if (n < 0 || n >= modulus) return 0;
		extend(n);
		return factorials[n];
	}
	Modular inverse_factorial(int n) const {
		synchronize();
		if (n < 0) return 0;
		if (n >= modulus) throw std::domain_error("factorial is not invertible");
		extend(n);
		return inverse_factorials[n];
	}
	Modular inverse(long long n) const {
		synchronize();
		if (0 < n && n < static_cast<long long>(factorials.size())) return inverse_factorials[n] * factorials[n - 1];
		return Modular(n).inverse();
	}
	// Uses Lucas for n >= modulus, with a multiplicative fallback for large digits.
	Modular binomial_coefficient(long long n, long long r) const {
		synchronize();
		if (r < 0 || r > n) return 0;
		return lucas(n, r);
	}
	Modular binomial_coefficient_large(long long n, long long r) const { return binomial_coefficient(n, r); }

	Modular permutation_count(long long n, long long r) const {
		synchronize();
		if (r < 0 || r > n) return 0;
		if (r == 0) return 1;
		const long long digit = n % modulus;
		if (r > digit) return 0;
		if (digit < cache_size_limit || digit < static_cast<long long>(factorials.size())) {
			extend(int(digit));
			return factorials[digit] * inverse_factorials[digit - r];
		}
		Modular result = 1;
		for (long long i = 0; i < r; ++i) result *= Modular(digit - i);
		return result;
	}
	Modular multiset_combination_count(long long n, long long r) const {
		synchronize();
		if (n < 0 || r < 0) return 0;
		if (n == 0) return r == 0 ? Modular(1) : Modular(0);
		return lucas(uint64_t(n) + uint64_t(r) - 1, r);
	}
	// Uses a binomial difference, so n + 1 need not be invertible.
	Modular catalan(long long n) const {
		synchronize();
		if (n < 0) return 0;
		const uint64_t twice = 2 * uint64_t(n);
		return lucas(twice, n) - (n == 0 ? Modular(0) : lucas(twice, n - 1));
	}
	Modular derangement(int n) const {
		synchronize();
		if (n < 0) return 0;
		Modular result = 1, sign = 1;
		for (long long i = 1; i <= n; ++i) {
			sign = -sign;
			result = result * Modular(i) + sign;
		}
		return result;
	}
};
