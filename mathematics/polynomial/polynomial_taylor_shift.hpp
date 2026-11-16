#pragma once
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Returns f(x+offset). O(M(n)) for n coefficients. Requires (n-1)! invertible.
template <typename Modular>
FormalPowerSeries<Modular> polynomial_taylor_shift(const FormalPowerSeries<Modular>& coefficients, Modular offset) {
	const int n = int(coefficients.size());
	if (n == 0) return {};
	Combinatorics<Modular> combinations(n - 1);
	std::vector<Modular> left(n), right(n);
	Modular power = 1;
	for (int i = 0; i < n; ++i) {
		left[n - 1 - i] = coefficients[i] * combinations.factorial(i);
		right[i] = power * combinations.inverse_factorial(i);
		power *= offset;
	}
	auto product = convolution(left, right);
	FormalPowerSeries<Modular> result(n);
	for (int i = 0; i < n; ++i) result[i] = product[n - 1 - i] * combinations.inverse_factorial(i);
	return result;
}
