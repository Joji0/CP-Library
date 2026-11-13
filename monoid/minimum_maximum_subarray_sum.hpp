#pragma once
#include <algorithm>
#include <cassert>
#include "monoid/detail/checked_arithmetic.hpp"

// Prefix, suffix, and subarray extrema use nonempty ranges.
template <typename T = long long>
struct MinimumMaximumSubarraySum {
	struct Bounds {
		T minimum{}, maximum{};
		bool operator==(const Bounds &) const = default;
	};
	struct value_type {
		T sum{};
		Bounds prefix, suffix, subarray;
		int count = 0;
		bool operator==(const value_type &) const = default;
	};
	static value_type identity() { return {}; }
	static value_type make_leaf(const T &value = T(0)) { return repeated(value, 1); }
	static value_type repeated(const T &value, int count) {
		assert(count >= 0);
		if (count == 0) return identity();
		const T sum = monoid_detail::checked_scale(value, count);
		const Bounds bounds{std::min(value, sum), std::max(value, sum)};
		return {sum, bounds, bounds, bounds, count};
	}

	static value_type combine(const value_type &left, const value_type &right) {
		if (left.count == 0) return right;
		if (right.count == 0) return left;
		const Bounds prefix = envelope(left.prefix, add({left.sum, left.sum}, right.prefix));
		const Bounds suffix = envelope(right.suffix, add(left.suffix, {right.sum, right.sum}));
		const Bounds subarray = envelope(envelope(left.subarray, right.subarray), add(left.suffix, right.prefix));
		return {monoid_detail::checked_add(left.sum, right.sum), prefix, suffix, subarray,
			monoid_detail::checked_add(left.count, right.count)};
	}

       private:
	static Bounds add(const Bounds &left, const Bounds &right) {
		return {monoid_detail::checked_add(left.minimum, right.minimum), monoid_detail::checked_add(left.maximum, right.maximum)};
	}
	static Bounds envelope(const Bounds &left, const Bounds &right) {
		return {std::min(left.minimum, right.minimum), std::max(left.maximum, right.maximum)};
	}
};
