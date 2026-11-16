#pragma once
#include <cassert>
#include <vector>
#include "mathematics/polynomial/polynomial_taylor_shift.hpp"

// Coefficient k of x(x+1)...(x+n-1) counts permutations into k cycles.
// Optional signed convention uses x(x-1)...(x-n+1). O(M(n)) for NTT-size products.
template <typename Modular>
std::vector<Modular> stirling_numbers_of_the_first_kind(int n, bool signed_coefficients = false) {
	assert(n >= 0);
	using Series = FormalPowerSeries<Modular>;
	auto build = [&](auto&& self, int count) -> Series {
		if (count == 0) return {Modular(1)};
		if (count == 1) return {Modular(0), Modular(1)};
		const int half = count / 2;
		auto lower = self(self, half);
		auto upper = polynomial_taylor_shift(lower, Modular(half));
		auto result = lower * upper;
		if (count & 1) {
			result.push_back(Modular(0));
			for (int i = count; i > 0; --i) result[i] = result[i - 1] + result[i] * Modular(count - 1);
			result[0] *= Modular(count - 1);
		}
		return result;
	};
	auto result = build(build, n);
	if (signed_coefficients) {
		for (int i = (n + 1) & 1; i <= n; i += 2) result[i] = -result[i];
	}
	return result;
}
