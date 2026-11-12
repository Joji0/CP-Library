#pragma once
#include "acted_monoid/minimum_maximum_affine.hpp"
#include "acted_monoid/sum_affine.hpp"

template <typename T = long long>
struct SumMinimumMaximumAffine {
	using Sum = SumAffine<T>;
	using Bounds = MinimumMaximumAffine<T>;
	using Action = typename Sum::Action;
	struct Value {
		typename Sum::Value sum;
		typename Bounds::Value extrema;
	};

	static Value identity() { return {Sum::identity(), Bounds::identity()}; }
	static Value make_leaf(const T &value = T(0)) { return {Sum::make_leaf(value), Bounds::make_leaf(value)}; }
	static Value combine(const Value &left, const Value &right) {
		return {Sum::combine(left.sum, right.sum), Bounds::combine(left.extrema, right.extrema)};
	}
	static Value mapping(Action action, const Value &aggregate) {
		return {Sum::mapping(action, aggregate.sum), Bounds::mapping({action.a, action.b}, aggregate.extrema)};
	}
	static Action composition(Action later, Action earlier) { return Sum::composition(later, earlier); }
	static Action identity_action() { return Sum::identity_action(); }
};
