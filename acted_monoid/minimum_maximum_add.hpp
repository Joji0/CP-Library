#pragma once
#include "acted_monoid/minimum_maximum_affine.hpp"
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MinimumMaximumAdd : MinimumMaximumAffine<T> {
	using Base = MinimumMaximumAffine<T>;
	using Value = typename Base::Value;
	using Action = T;

	static Value mapping(Action action, Value value) { return Base::mapping({T(1), action}, value); }
	static Action composition(Action later, Action earlier) { return monoid_detail::checked_add(later, earlier); }
	static Action identity_action() { return T(0); }
};
