#pragma once
#include "mathematics/set_function/subset_convolution.hpp"

// Sum over partitions of each mask, with block weight series[block].
// Requires series[0] = 0. No division; works in positive characteristic.
template <typename Value>
std::vector<Value> set_power_series_exponential(const std::vector<Value>& series) {
	if (!std::has_single_bit(series.size()) || series[0] != Value(0)) throw std::invalid_argument("expected zero constant term");
	std::vector<Value> result{Value(1)};
	for (std::size_t bit = 1; bit < series.size(); bit <<= 1) {
		// Every nonempty mask has a unique block containing its highest bit.
		auto upper = subset_convolution(result, std::vector<Value>(series.begin() + bit, series.begin() + 2 * bit));
		result.insert(result.end(), upper.begin(), upper.end());
	}
	return result;
}
