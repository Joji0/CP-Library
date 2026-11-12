#pragma once
#include <cstdint>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct SumAdd {
	struct Value {
		T val;
		int cnt;
	};
	using Action = T;
	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(Value a, Value b) { return {monoid_detail::checked_add(a.val, b.val), monoid_detail::checked_add(a.cnt, b.cnt)}; }
	static Value identity() { return {0, 0}; }
	static Value mapping(Action f, Value x) {
		if (x.cnt == 0) return x;
		return {monoid_detail::checked_scaled_add(f, x.cnt, x.val), x.cnt};
	}
	static Action composition(Action f, Action g) { return monoid_detail::checked_add(f, g); }
	static Action identity_action() { return 0; }
};
