#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MaximumSubarraySum {
	struct Node {
		T sum;
		T pref;
		T suff;
		T ans;
		bool empty = false;
	};
	using value_type = Node;
	static value_type identity() {
		return {0, 0, 0, 0, true};
	}
	static value_type from_val(T x) {
		return {x, x, x, x};
	}
	static value_type combine(const value_type &a, const value_type &b) {
		if (a.empty) return b;
		if (b.empty) return a;
		value_type res;
		res.sum = monoid_detail::checked_add(a.sum, b.sum);
		res.pref = std::max(a.pref, monoid_detail::checked_add(a.sum, b.pref));
		res.suff = std::max(b.suff, monoid_detail::checked_add(b.sum, a.suff));
		res.ans = std::max({a.ans, b.ans, monoid_detail::checked_add(a.suff, b.pref)});
		return res;
	}
};
