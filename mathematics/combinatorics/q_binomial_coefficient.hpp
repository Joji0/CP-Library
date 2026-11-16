#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "mathematics/combinatorics.hpp"

// Gaussian binomial at base q over a prime field. Uses q-Lucas when the
// multiplicative order is small. Construction and queries use the same modulus.
template <typename Modular>
class QBinomialCoefficient {
	long long modulus, order;
	Modular base;
	Combinatorics<Modular> ordinary;
	std::vector<Modular> products, inverse_products;

       public:
	explicit QBinomialCoefficient(Modular base, int maximum_hint = 0)
	    : modulus(Modular::get_modulus()), order(modulus - 1), base(base), ordinary(0) {
		if (base == Modular(0)) return;
		if (base == Modular(1)) {
			ordinary.reserve(maximum_hint);
			return;
		}
		for (auto factor : NumberTheory::factorize(order)) {
			if (order % factor == 0 && base.power(order / factor) == Modular(1)) order /= factor;
		}
		ordinary.reserve(int(std::max(0LL, maximum_hint / order)));
		const int limit = int(std::max(0LL, std::min<long long>(order - 1, maximum_hint)));
		products.resize(limit + 1, Modular(1));
		inverse_products.resize(limit + 1, Modular(1));
		Modular power = 1;
		for (int i = 1; i <= limit; ++i) {
			power *= base;
			products[i] = products[i - 1] * (Modular(1) - power);
		}
		inverse_products.back() = products.back().inverse();
		const Modular inverse_base = base.inverse();
		for (int i = limit; i > 0; --i) {
			inverse_products[i - 1] = inverse_products[i] * (Modular(1) - power);
			power *= inverse_base;
		}
	}

	Modular binomial_coefficient(long long n, long long r) const {
		if (Modular::get_modulus() != modulus) throw std::logic_error("q-binomial modulus changed");
		if (r < 0 || r > n) return 0;
		if (base == Modular(0)) return 1;
		if (base == Modular(1)) return ordinary.binomial_coefficient(n, r);
		Modular result = ordinary.binomial_coefficient(n / order, r / order);
		n %= order;
		r %= order;
		if (r > n || result == Modular(0)) return 0;
		if (n < static_cast<long long>(products.size())) {
			return result * products[n] * inverse_products[r] * inverse_products[n - r];
		}
		r = std::min(r, n - r);
		Modular numerator = 1, denominator = 1;
		Modular top = base.power(n - r), bottom = 1;
		for (long long i = 1; i <= r; ++i) {
			top *= base;
			bottom *= base;
			numerator *= Modular(1) - top;
			denominator *= Modular(1) - bottom;
		}
		return result * numerator / denominator;
	}
};
