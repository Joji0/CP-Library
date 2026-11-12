#pragma once
#include <utility>

template <typename First, typename Second>
struct PairMonoid {
	using value_type = std::pair<typename First::value_type, typename Second::value_type>;
	static value_type identity() { return {First::identity(), Second::identity()}; }
	static value_type combine(const value_type &left, const value_type &right) {
		return {First::combine(left.first, right.first), Second::combine(left.second, right.second)};
	}
};
