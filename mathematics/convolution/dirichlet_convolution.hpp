#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>

// result[n] = sum_{d | n} left[d] * right[n/d] for 1 <= n <= maximum.
// The zero slots are ignored; entries beyond either input's size are zero.
// O(maximum log(maximum+1)) time.
template <typename Value>
std::vector<Value> dirichlet_convolution(const std::vector<Value>& left, const std::vector<Value>& right, int maximum) {
	if (left.empty() || right.empty() || maximum < 0) throw std::invalid_argument("expected tables with dummy slot and nonnegative limit");
	std::vector<Value> result(std::size_t(maximum) + 1, Value(0));
	for (std::size_t i = 1; i < left.size() && i <= std::size_t(maximum); ++i) {
		const std::size_t end = std::min(right.size() - 1, maximum / i);
		for (std::size_t j = 1; j <= end; ++j) result[i * j] += left[i] * right[j];
	}
	return result;
}
