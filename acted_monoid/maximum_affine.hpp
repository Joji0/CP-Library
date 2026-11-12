#pragma once
#include <algorithm>
#include <cassert>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MaximumAffine {
	using Value = std::optional<T>;
	struct Action {
		T a, b;
	};

	static Value make_leaf(const T &value = T(0)) { return value; }
	static Value identity() { return std::nullopt; }
	static Value combine(Value first, Value second) {
		if (!first) return second;
		if (!second) return first;
		return std::max(*first, *second);
	}

	// A single extremum can be mapped only by a nondecreasing affine function.
	static Value mapping(Action action, Value value) {
		assert(!(action.a < T(0)));
		if (!value) return std::nullopt;
		return monoid_detail::checked_multiply_add(action.a, *value, action.b);
	}
	static Action composition(Action later, Action earlier) {
		assert(!(later.a < T(0)) && !(earlier.a < T(0)));
		return {monoid_detail::checked_multiply(later.a, earlier.a), monoid_detail::checked_multiply_add(later.a, earlier.b, later.b)};
	}
	static Action identity_action() { return {T(1), T(0)}; }
};
