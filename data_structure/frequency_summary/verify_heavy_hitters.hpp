#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include "monoid/detail/checked_arithmetic.hpp"

// count(key) must return the exact frequency in the range summarized.
template <typename Summary, typename Count>
std::vector<std::pair<int, long long>> verify_heavy_hitters(const Summary &summary, long long minimum_frequency,
							    const Count &count) {
	if (summary.length == 0) return {};
	if (minimum_frequency <= summary.length / (summary.capacity() + 1LL)) monoid_detail::arithmetic_failure();
	std::vector<std::pair<int, long long>> result;
	for (int key : summary.candidates()) {
		const long long frequency = count(key);
		if (frequency >= minimum_frequency) result.emplace_back(key, frequency);
	}
	std::sort(result.begin(), result.end());
	return result;
}
