#pragma once
#include "monoid/detail/checked_arithmetic.hpp"
#include "monoid/multiply.hpp"
#include "monoid/add.hpp"

template <typename T = long long>
struct SumMultiply : Add<T> {
	using Value = T;
	using Action = T;

	static Value make_leaf(const T &value = T(0)) { return value; }
	static Value mapping(Action action, Value aggregate) { return monoid_detail::checked_multiply(action, aggregate); }
	static Action composition(Action later, Action earlier) { return Multiply<T>::combine(later, earlier); }
	static Action identity_action() { return T(1); }
};
