#pragma once
#include "monoid/detail/checked_arithmetic.hpp"
#include "monoid/minimum_maximum.hpp"

// Stores both extrema so an affine update with a negative slope can swap them.
template <typename T = long long>
struct MinimumMaximumAffine : MinimumMaximum<T> {
	using Base = MinimumMaximum<T>;
	using Extrema = typename Base::Extrema;
	using Value = typename Base::value_type;

	struct Action {
		T a, b;
	};

	static Value make_leaf(const T &value = T(0)) { return Base::make_leaf(value); }

	static Value mapping(Action action, Value value) {
		if (!value) return std::nullopt;
		const T first = monoid_detail::checked_multiply_add(action.a, value->minimum, action.b);
		const T second = monoid_detail::checked_multiply_add(action.a, value->maximum, action.b);
		if (action.a < T(0)) return Extrema{second, first};
		return Extrema{first, second};
	}

	static Action composition(Action later, Action earlier) {
		return {monoid_detail::checked_multiply(later.a, earlier.a), monoid_detail::checked_multiply_add(later.a, earlier.b, later.b)};
	}
	static Action identity_action() { return {T(1), T(0)}; }
};
