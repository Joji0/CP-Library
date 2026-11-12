#pragma once
#include <optional>
#include "monoid/minimum_maximum.hpp"

template <typename T = long long>
struct MinimumMaximumAssign : MinimumMaximum<T> {
	using Base = MinimumMaximum<T>;
	using Value = typename Base::value_type;
	using Action = std::optional<T>;

	static Value make_leaf(const T &value = T(0)) { return Base::make_leaf(value); }
	static Value mapping(Action action, Value value) { return action && value ? make_leaf(*action) : value; }
	static Action composition(Action later, Action earlier) { return later ? later : earlier; }
	static Action identity_action() { return std::nullopt; }
};
