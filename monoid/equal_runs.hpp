#pragma once
#include <algorithm>
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_f
// Longest contiguous run of equal values, including the runs at both boundaries.
template <typename T>
struct EqualRuns {
	struct value_type {
		T first{}, last{};
		int length = 0, prefix = 0, suffix = 0, longest = 0;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_leaf(const T &value) { return {value, value, 1, 1, 1, 1}; }

	static value_type combine(const value_type &left, const value_type &right) {
		if (left.length == 0) return right;
		if (right.length == 0) return left;
		value_type result{left.first, right.last,
				  monoid_detail::checked_add(left.length, right.length), left.prefix, right.suffix,
				  std::max(left.longest, right.longest)};
		if (left.last == right.first) {
			result.longest = std::max(result.longest, left.suffix + right.prefix);
			if (left.prefix == left.length) result.prefix += right.prefix;
			if (right.suffix == right.length) result.suffix += left.suffix;
		}
		return result;
	}
};
