#pragma once
#include "monoid/add.hpp"
#include "monoid/minimum_maximum.hpp"
#include "monoid/pair_monoid.hpp"

template <typename T = long long>
struct SumMinimumMaximum : PairMonoid<Add<T>, MinimumMaximum<T>> {
	using Base = PairMonoid<Add<T>, MinimumMaximum<T>>;
	using value_type = typename Base::value_type;
	static value_type make_leaf(const T &value) { return {value, MinimumMaximum<T>::make_leaf(value)}; }
};
