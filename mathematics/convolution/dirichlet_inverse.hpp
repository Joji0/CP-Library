#pragma once
#include <stdexcept>
#include <vector>

// Multiplicative inverse under Dirichlet convolution up to series.size()-1.
// series[1] must be a unit. Slot 0 is ignored. O(n log n) time.
template <typename Value>
std::vector<Value> dirichlet_inverse(const std::vector<Value>& series) {
	if (series.size() < 2 || series[1] == Value(0)) throw std::invalid_argument("Dirichlet inverse requires a unit at index 1");
	const Value inverse = Value(1) / series[1];
	std::vector<Value> result(series.size(), Value(0));
	result[1] = inverse;
	for (std::size_t i = 1; i < series.size(); ++i) {
		if (i > 1) result[i] = -result[i] * inverse;
		for (std::size_t j = 2; j <= (series.size() - 1) / i; ++j) result[i * j] += result[i] * series[j];
	}
	return result;
}
