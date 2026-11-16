#pragma once
#include "mathematics/set_function/subset_convolution.hpp"

// Multiplicative inverse under subset convolution. series[0] must be a unit.
template <typename Value>
std::vector<Value> set_power_series_inverse(const std::vector<Value>& series) {
	if (!std::has_single_bit(series.size())) throw std::invalid_argument("expected one value per bitmask");
	std::vector<Value> result{Value(1) / series[0]};
	for (std::size_t bit = 1; bit < series.size(); bit <<= 1) {
		auto upper = subset_convolution(result, std::vector<Value>(series.begin() + bit, series.begin() + 2 * bit));
		upper = subset_convolution(result, upper);
		for (auto value : upper) result.push_back(-value);
	}
	return result;
}
