#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Bernoulli Numbers and Faulhaber's Formula for sum_{i=0}^{n-1} i^k in O(K log K) or O(K).
template <typename Mint = ModularInteger998244353>
class BernoulliNumber {
	Combinatorics<Mint> _comb;
	std::vector<Mint> _b;

       public:
	explicit BernoulliNumber(int maximum) : _comb(maximum + 1), _b(maximum + 1) {
		FormalPowerSeries<Mint> den(maximum + 1);
		for (int i = 0; i <= maximum; ++i) {
			den[i] = _comb.inverse_factorial(i + 1);
		}
		FormalPowerSeries<Mint> gf = den.inverse(maximum + 1);
		for (int i = 0; i <= maximum; ++i) {
			_b[i] = gf[i] * _comb.factorial(i);
		}
	}

	int maximum() const { return int(_b.size()) - 1; }
	const std::vector<Mint>& numbers() const { return _b; }
	Mint number(int deg) const {
		assert(0 <= deg && deg <= maximum());
		return _b[deg];
	}

	// Faulhaber: sum_{i=0}^{n-1} i^deg
	Mint power_sum(Mint n, int deg) const {
		assert(0 <= deg && deg <= maximum());
		std::vector<Mint> powers(deg + 2, Mint(1));
		for (int i = 0; i <= deg; ++i) powers[i + 1] = powers[i] * n;

		Mint res = 0;
		for (int i = 0; i <= deg; ++i) {
			res += _comb.binomial_coefficient(deg + 1, i) * _b[i] * powers[deg + 1 - i];
		}
		return res * _comb.inverse(deg + 1);
	}

	// Inclusive range. An empty range contributes zero, even if endpoints
	// coincide modulo the coefficient modulus after conversion.
	Mint power_sum(long long left, long long right, int deg) const {
		if (left > right) return 0;
		return power_sum(Mint(right) + Mint(1), deg) - power_sum(Mint(left), deg);
	}
};
