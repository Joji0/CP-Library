#pragma once

template <typename Monoid>
struct ReverseMonoid {
	using value_type = typename Monoid::value_type;
	static value_type identity() { return Monoid::identity(); }
	static value_type combine(const value_type &left, const value_type &right) {
		return Monoid::combine(right, left);
	}
};
