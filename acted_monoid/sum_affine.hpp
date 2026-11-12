#pragma once
#include <cstdint>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct SumAffine {
	struct Value {
		T val;
		int cnt;
	};
	struct Action {
		T a, b;
	};
	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(Value x, Value y) { return {monoid_detail::checked_add(x.val, y.val), monoid_detail::checked_add(x.cnt, y.cnt)}; }
	static Value identity() { return {0, 0}; }
	static Value mapping(Action f, Value x) {
		if (x.cnt == 0) return x;
		return {monoid_detail::checked_linear_combination(f.a, x.val, f.b, x.cnt), x.cnt};
	}
	static Action composition(Action f, Action g) { return {monoid_detail::checked_multiply(f.a, g.a), monoid_detail::checked_multiply_add(f.a, g.b, f.b)}; }
	static Action identity_action() { return {1, 0}; }
};
