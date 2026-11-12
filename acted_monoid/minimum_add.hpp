#pragma once
#include <algorithm>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MinimumAdd {
	using Value = std::optional<T>;
	using Action = T;

	static Value make_leaf(const T &value = T(0)) { return value; }
	static Value identity() { return std::nullopt; }
	static Value combine(Value first, Value second) {
		if (!first) return second;
		if (!second) return first;
		return std::min(*first, *second);
	}

	static Value mapping(Action action, Value value) {
		if (!value) return std::nullopt;
		return monoid_detail::checked_add(*value, action);
	}
	static Action composition(Action later, Action earlier) { return monoid_detail::checked_add(later, earlier); }
	static Action identity_action() { return T(0); }
};
