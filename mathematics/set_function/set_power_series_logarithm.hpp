#pragma once
#include "mathematics/set_function/subset_convolution.hpp"

// Inverse of the set exponential. Requires series[0] = 1. No division.
template <typename Value>
std::vector<Value> set_power_series_logarithm(const std::vector<Value>& series) {
	if (!std::has_single_bit(series.size()) || series[0] != Value(1)) throw std::invalid_argument("expected constant term 1");
	std::vector<Value> inverse{Value(1)}, result{Value(0)};
	for (std::size_t bit = 1; bit < series.size(); bit <<= 1) {
		auto upper = subset_convolution(inverse, std::vector<Value>(series.begin() + bit, series.begin() + 2 * bit));
		result.insert(result.end(), upper.begin(), upper.end());
		if (2 * bit < series.size()) {
			upper = subset_convolution(inverse, upper);
			for (auto value : upper) inverse.push_back(-value);
		}
	}
	return result;
}
