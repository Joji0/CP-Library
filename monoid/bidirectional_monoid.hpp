#pragma once

template <typename Monoid>
struct BidirectionalMonoid {
	using Aggregate = typename Monoid::value_type;
	struct value_type {
		Aggregate forward, backward;
	};

	static value_type identity() { return {Monoid::identity(), Monoid::identity()}; }
	static value_type make_leaf(const Aggregate &value) { return {value, value}; }
	static value_type combine(const value_type &left, const value_type &right) {
		return {Monoid::combine(left.forward, right.forward), Monoid::combine(right.backward, left.backward)};
	}
};
