#pragma once
#include <array>
#include <cstddef>
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_e
// Weighted counts for each contiguous fragment of Pattern. A subsequence's weight
// is the product of its selected positions' weights. Example Pattern: std::array{'R','G','B'}.
template <auto Pattern, typename Weight = long long>
struct WeightedPatternSubsequences {
	static constexpr std::size_t length = Pattern.size();
	static_assert(length > 0);
	using value_type = std::array<std::array<Weight, length>, length>;

	static value_type identity() { return {}; }

	template <typename Symbol>
	static value_type make_leaf(const Symbol &symbol, const Weight &weight = Weight(1)) {
		value_type result{};
		for (std::size_t i = 0; i < length; ++i) {
			if (Pattern[i] == symbol) result[i][i] = weight;
		}
		return result;
	}

	static value_type combine(const value_type &left, const value_type &right) {
		using namespace monoid_detail;
		value_type result{};
		for (std::size_t first = 0; first < length; ++first) {
			for (std::size_t last = first; last < length; ++last) {
				Weight count = checked_add(left[first][last], right[first][last]);
				for (std::size_t middle = first; middle < last; ++middle) {
					count = checked_multiply_add(left[first][middle], right[middle + 1][last], count);
				}
				result[first][last] = count;
			}
		}
		return result;
	}

	static Weight weighted_count(const value_type &value) { return value[0][length - 1]; }
};
