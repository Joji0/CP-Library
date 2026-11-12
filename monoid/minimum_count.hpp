#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MinimumCount {
	struct Node {
		T val;
		long long count;
	};
	using value_type = Node;
	static value_type identity() { return {std::numeric_limits<T>::max(), 0}; }
	static value_type combine(const value_type &a, const value_type &b) {
		if (a.count == 0) return b;
		if (b.count == 0) return a;
		if (a.val < b.val) return a;
		if (b.val < a.val) return b;
		return {a.val, monoid_detail::checked_add(a.count, b.count)};
	}
};
