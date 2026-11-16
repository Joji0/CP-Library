#pragma once
#include <cassert>
#include <cstdint>
#include <vector>
#include "mathematics/formal_power_series/bostan_mori.hpp"

// a[n] = c[0]*a[n-1] + ... + c[d-1]*a[n-d]. a[0..d-1] are initial terms.
// O(M(d) log(index+1)) time. An order-0 recurrence is the zero sequence.
template <typename Modular>
Modular linear_recurrence_term(const std::vector<Modular>& initial, const std::vector<Modular>& coefficients, uint64_t index) {
	return BostanMori<Modular>::solve(initial, coefficients, index);
}

// Sum a[0] + ... + a[index], including a[index].
template <typename Modular>
Modular linear_recurrence_prefix_sum(const std::vector<Modular>& initial, const std::vector<Modular>& coefficients, uint64_t index) {
	using Series = FormalPowerSeries<Modular>;
	assert(initial.size() == coefficients.size());
	const int order = int(initial.size());
	if (order == 0) return 0;
	Series denominator(order + 1, Modular(1));
	for (int i = 0; i < order; ++i) denominator[i + 1] = -coefficients[i];
	auto numerator = (Series(initial) * denominator).prefix(order);
	denominator *= Series{Modular(1), Modular(-1)};
	return BostanMori<Modular>::coefficient_of_rational(numerator, denominator, index);
}
