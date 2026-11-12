#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MaximumCountAdd {
	struct Value {
		T val;
		long long cnt;
	};
	using Action = T;
	static constexpr T NEG_INF = std::numeric_limits<T>::lowest();

	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(Value a, Value b) {
		if (a.cnt == 0) return b;
		if (b.cnt == 0) return a;
		if (a.val > b.val) return a;
		if (b.val > a.val) return b;
		return {a.val, monoid_detail::checked_add(a.cnt, b.cnt)};
	}
	static Value identity() { return {NEG_INF, 0}; }
	static Value mapping(Action f, Value x) {
		if (x.cnt == 0) return x;
		return {monoid_detail::checked_add(x.val, f), x.cnt};
	}
	static Action composition(Action f, Action g) { return monoid_detail::checked_add(f, g); }
	static Action identity_action() { return 0; }
};
