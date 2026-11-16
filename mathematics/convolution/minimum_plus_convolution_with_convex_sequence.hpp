#pragma once
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>

// Only the second sequence must be convex (nondecreasing first differences).
// Dense input indexed by degree. Monotone row minima in O((n+m) log(n+m)).
inline std::vector<long long> minimum_plus_convolution_with_convex_sequence(
    const std::vector<long long>& arbitrary, const std::vector<long long>& convex) {
	if (arbitrary.empty() || convex.empty()) return {};
	if (arbitrary.size() + convex.size() > std::numeric_limits<int>::max()) {
		throw std::length_error("min-plus convolution size exceeds int");
	}
	const int n = int(arbitrary.size()), m = int(convex.size());
	std::vector<long long> result(n + m - 1);
	auto solve = [&](auto&& self, int left, int right, int first, int last) -> void {
		if (left > right) return;
		const int middle = left + (right - left) / 2;
		int best = std::max(first, middle - m + 1);
		__int128 minimum = __int128(arbitrary[best]) + convex[middle - best];
		for (int i = best + 1; i <= std::min(last, middle); ++i) {
			const __int128 value = __int128(arbitrary[i]) + convex[middle - i];
			if (value < minimum) {
				minimum = value;
				best = i;
			}
		}
		if (minimum < std::numeric_limits<long long>::min() || minimum > std::numeric_limits<long long>::max()) {
			throw std::overflow_error("min-plus convolution result exceeds long long");
		}
		result[middle] = static_cast<long long>(minimum);
		self(self, left, middle - 1, first, best);
		self(self, middle + 1, right, best, last);
	};
	solve(solve, 0, int(result.size()) - 1, 0, n - 1);
	return result;
}
