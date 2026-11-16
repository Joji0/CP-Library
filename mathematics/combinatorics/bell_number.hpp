#pragma once
#include <cassert>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Entry n counts all set partitions of n labeled objects. EGF exp(exp(x)-1).
// Requires maximum < prime modulus.
template <typename Modular>
std::vector<Modular> bell_numbers(int maximum) {
	assert(maximum >= 0);
	Combinatorics<Modular> combinations(maximum);
	FormalPowerSeries<Modular> series(maximum + 1);
	for (int i = 1; i <= maximum; ++i) series[i] = combinations.inverse_factorial(i);
	auto result = series.exponential();
	for (int i = 0; i <= maximum; ++i) result[i] *= combinations.factorial(i);
	return result;
}
