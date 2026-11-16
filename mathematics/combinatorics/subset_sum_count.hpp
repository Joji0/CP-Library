#pragma once
#include <cassert>
#include <stdexcept>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Count subsets of labeled items by sum. values[0] is ignored. Values are
// nonnegative (zero doubles all counts). Entry s answers sum s, including s=0.
// O(values.size() + maximum * log(maximum+1) + M(maximum)) over a prime field.
template <typename Modular>
std::vector<Modular> subset_sum_counts(const std::vector<int>& values, int maximum) {
	assert(maximum >= 0 && !values.empty());
	Combinatorics<Modular> combinations(maximum);
	std::vector<int> frequency(maximum + 1);
	for (std::size_t i = 1; i < values.size(); ++i) {
		if (values[i] < 0) throw std::invalid_argument("subset sum requires nonnegative values");
		if (values[i] <= maximum) ++frequency[values[i]];
	}
	FormalPowerSeries<Modular> logarithm(maximum + 1);
	std::vector<Modular> inverse(maximum + 1);
	for (int i = 1; i <= maximum; ++i) inverse[i] = combinations.inverse_factorial(i) * combinations.factorial(i - 1);
	for (int value = 1; value <= maximum; ++value) {
		if (frequency[value] == 0) continue;
		const Modular count = frequency[value];
		for (int times = 1; times <= maximum / value; ++times) {
			const auto term = count * inverse[times];
			logarithm[value * times] += (times & 1) ? term : -term;
		}
	}
	return logarithm.exponential() * Modular(2).power(frequency[0]);
}
