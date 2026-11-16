#pragma once
#include <algorithm>
#include <bit>
#include <cstddef>
#include <stdexcept>
#include <vector>

// result[mask] = sum(left[submask] * right[mask ^ submask]).
// Inputs contain exactly 2^k values. O(k^2 2^k) time and O(k 2^k) memory.
template <typename Value>
std::vector<Value> subset_convolution(const std::vector<Value>& left, const std::vector<Value>& right) {
	const std::size_t count = left.size();
	if (count != right.size() || !std::has_single_bit(count)) throw std::invalid_argument("incompatible bitmask tables");
	const int rank_count = std::bit_width(count);
	std::vector<Value> left_rank(count * rank_count, Value(0)), right_rank(count * rank_count, Value(0));
	for (std::size_t mask = 0; mask < count; ++mask) {
		left_rank[mask * rank_count + std::popcount(mask)] = left[mask];
		right_rank[mask * rank_count + std::popcount(mask)] = right[mask];
	}
	for (std::size_t bit = 1; bit < count; bit <<= 1) {
		for (std::size_t mask = 0; mask < count; ++mask) {
			if (!(mask & bit)) continue;
			const int limit = std::popcount(mask) - 1;
			for (int rank = 0; rank <= limit; ++rank) {
				left_rank[mask * rank_count + rank] += left_rank[(mask ^ bit) * rank_count + rank];
				right_rank[mask * rank_count + rank] += right_rank[(mask ^ bit) * rank_count + rank];
			}
		}
	}
	for (std::size_t mask = 0; mask < count; ++mask) {
		auto* lhs = left_rank.data() + mask * rank_count;
		const auto* rhs = right_rank.data() + mask * rank_count;
		const int size = std::popcount(mask);
		for (int rank = std::min(2 * size, rank_count - 1); rank >= 0; --rank) {
			Value value = 0;
			for (int i = std::max(0, rank - size); i <= std::min(rank, size); ++i) value += lhs[i] * rhs[rank - i];
			lhs[rank] = value;
		}
	}
	for (std::size_t bit = 1; bit < count; bit <<= 1) {
		for (std::size_t mask = 0; mask < count; ++mask) {
			if (!(mask & bit)) continue;
			const int limit = std::min(2 * (std::popcount(mask) - 1), rank_count - 1);
			for (int rank = 0; rank <= limit; ++rank) {
				left_rank[mask * rank_count + rank] -= left_rank[(mask ^ bit) * rank_count + rank];
			}
		}
	}
	std::vector<Value> result(count);
	for (std::size_t mask = 0; mask < count; ++mask) result[mask] = left_rank[mask * rank_count + std::popcount(mask)];
	return result;
}
