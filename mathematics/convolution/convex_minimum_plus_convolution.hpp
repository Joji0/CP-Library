#pragma once
#include <cassert>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

// Min-plus convolution of two convex sequences (nondecreasing first differences).
// Dense input indexed by degree; returns min(a[i] + b[k-i]) for each k. O(n+m).
inline std::vector<long long> convex_minimum_plus_convolution(
    const std::vector<long long>& left, const std::vector<long long>& right) {
	if (left.empty() || right.empty()) return {};
	std::vector<long long> result;
	result.reserve(left.size() + right.size() - 1);
	std::size_t i = 0, j = 0;
	while (true) {
		const __int128 value = __int128(left[i]) + right[j];
		if (value < std::numeric_limits<long long>::min() || value > std::numeric_limits<long long>::max()) {
			throw std::overflow_error("min-plus convolution result exceeds long long");
		}
		result.push_back(static_cast<long long>(value));
		if (i + 1 == left.size() && j + 1 == right.size()) break;
		const bool take_left = i + 1 < left.size() &&
				       (j + 1 == right.size() || __int128(left[i + 1]) - left[i] <= __int128(right[j + 1]) - right[j]);
		if (take_left)
			++i;
		else
			++j;
	}
	return result;
}
