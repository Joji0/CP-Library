#pragma once
#include <cassert>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Entry n is S(n,blocks), for 0 <= n <= maximum. O(M(maximum-blocks)+maximum)
// over a prime field with maximum < modulus.
template <typename Modular>
std::vector<Modular> stirling_numbers_of_the_second_kind_fixed_blocks(int maximum, int blocks) {
	assert(maximum >= 0 && blocks >= 0);
	std::vector<Modular> result(maximum + 1);
	if (blocks > maximum) return result;
	if (blocks == 0) {
		result[0] = 1;
		return result;
	}
	Combinatorics<Modular> combinations(maximum);
	const int size = maximum - blocks + 1;
	FormalPowerSeries<Modular> series(size);
	for (int i = 0; i < size; ++i) series[i] = combinations.inverse_factorial(i + 1);
	auto power = series.power(blocks, size);
	const auto inverse = combinations.inverse_factorial(blocks);
	for (int i = blocks; i <= maximum; ++i) result[i] = power[i - blocks] * combinations.factorial(i) * inverse;
	return result;
}
