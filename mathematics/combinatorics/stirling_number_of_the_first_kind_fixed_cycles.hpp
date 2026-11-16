#pragma once
#include <cassert>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Entry n counts permutations with n elements and exactly cycles cycles.
// Uses (-log(1-x))^cycles / cycles! as an EGF; requires maximum < prime modulus.
template <typename Modular>
std::vector<Modular> stirling_numbers_of_the_first_kind_fixed_cycles(int maximum, int cycles, bool signed_coefficients = false) {
	assert(maximum >= 0 && cycles >= 0);
	std::vector<Modular> result(maximum + 1);
	if (cycles > maximum) return result;
	if (cycles == 0) {
		result[0] = 1;
		return result;
	}
	Combinatorics<Modular> combinations(maximum);
	const int size = maximum - cycles + 1;
	FormalPowerSeries<Modular> series(size);
	for (int i = 0; i < size; ++i) series[i] = combinations.inverse(i + 1);
	auto power = series.power(cycles, size);
	const auto inverse = combinations.inverse_factorial(cycles);
	for (int i = cycles; i <= maximum; ++i) {
		result[i] = power[i - cycles] * combinations.factorial(i) * inverse;
		if (signed_coefficients && ((i - cycles) & 1)) result[i] = -result[i];
	}
	return result;
}
