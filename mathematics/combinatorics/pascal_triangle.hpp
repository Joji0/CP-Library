#pragma once
#include <cassert>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Row and column are counts, so C(n,0) lives at [n][0]. O(n^2).
template <typename Value = long long>
std::vector<std::vector<Value>> pascal_triangle(int maximum) {
	assert(maximum >= 0 && maximum < std::numeric_limits<int>::max());
	std::vector<std::vector<Value>> result(maximum + 1);
	for (int n = 0; n <= maximum; ++n) {
		result[n].assign(n + 1, Value(1));
		for (int k = 1; k < n; ++k) {
			if constexpr (std::is_integral_v<Value> && !std::is_same_v<Value, bool>) {
				if (__builtin_add_overflow(result[n - 1][k - 1], result[n - 1][k], &result[n][k])) {
					throw std::overflow_error("Pascal triangle entry exceeds result type");
				}
			} else {
				result[n][k] = result[n - 1][k - 1] + result[n - 1][k];
			}
		}
	}
	return result;
}
