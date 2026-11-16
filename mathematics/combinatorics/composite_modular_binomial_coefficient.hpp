#pragma once
#include <cstdint>
#include <stdexcept>
#include <vector>
#include "mathematics/combinatorics/prime_power_binomial_coefficient.hpp"

// Works for any positive signed 64-bit modulus, including 1 and primes.
// Factorizes once, then combines prime-power residues using precomputed CRT weights.
class CompositeModularBinomialCoefficient {
	long long modulus;
	std::vector<PrimePowerBinomialCoefficient> pieces;
	std::vector<long long> weights;

       public:
	explicit CompositeModularBinomialCoefficient(long long modulus, long long table_limit = 2'000'000) : modulus(modulus) {
		if (modulus <= 0) throw std::invalid_argument("modulus must be positive");
		auto factors = NumberTheory::factorize(modulus);
		for (std::size_t i = 0; i < factors.size();) {
			std::size_t j = i + 1;
			while (j < factors.size() && factors[j] == factors[i]) ++j;
			pieces.emplace_back(factors[i], int(j - i), table_limit);
			const long long component = pieces.back().get_modulus();
			const long long other = modulus / component;
			auto inverse = *NumberTheory::modular_inverse(other, component);
			weights.push_back(__int128(other) * inverse % modulus);
			i = j;
		}
	}
	long long get_modulus() const { return modulus; }
	long long binomial_coefficient(long long n, long long r) const {
		long long result = 0;
		for (std::size_t i = 0; i < pieces.size(); ++i) {
			result = (__int128(result) + __int128(weights[i]) * pieces[i].binomial_coefficient(n, r)) % modulus;
		}
		return result;
	}
};
