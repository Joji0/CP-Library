#pragma once
#include <cassert>
#include <cstddef>
#include <optional>
#include "monoid/maximum_plus_matrix.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_i
// Maximum total over 1..Count disjoint nonempty subarrays. Adjacent subarrays are allowed.
// Leaves and monoid products are DP transition matrices.
template <typename T, std::size_t Count>
struct MaximumDisjointSubarraySum : MaximumPlusMatrix<T, 2 * Count + 1> {
	static_assert(Count > 0);
	using Base = MaximumPlusMatrix<T, 2 * Count + 1>;
	using typename Base::value_type;

	static value_type make_leaf(const T &value) {
		value_type result = Base::identity();
		for (std::size_t selected = 1; selected <= Count; ++selected) {
			const std::size_t open = 2 * selected;
			result[open - 1][open] = value;
			result[open][open] = value;
			result[open][open + 1] = T(0);
			if (selected > 1) result[open - 2][open] = value;
		}
		return result;
	}

	static std::optional<T> maximum_sum(const value_type &value, std::size_t minimum = 1, std::size_t maximum = Count) {
		assert(minimum <= maximum && maximum <= Count);
		std::optional<T> result;
		if (minimum == 0) {
			result = T(0);
			minimum = 1;
		}
		for (std::size_t state = 2 * minimum; state <= 2 * maximum + 1; ++state) {
			if (value[1][state] && (!result || *result < *value[1][state])) result = value[1][state];
		}
		return result;
	}
};
