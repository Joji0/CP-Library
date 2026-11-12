#pragma once
#include <cstdint>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct FlipSum {
	struct Value {
		T sum;
		long long cnt;
	};
	using Action = bool;
	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(Value a, Value b) { return {monoid_detail::checked_add(a.sum, b.sum), monoid_detail::checked_add(a.cnt, b.cnt)}; }
	static Value identity() { return {0, 0}; }
	static Value mapping(Action f, Value x) { return f ? Value{monoid_detail::checked_subtract(monoid_detail::checked_count<T>(x.cnt), x.sum), x.cnt} : x; }
	static Action composition(Action f, Action g) { return f ^ g; }
	static Action identity_action() { return false; }
};
