#pragma once
#include "data_structure/union_find/detail/potential_groups.hpp"
#include "data_structure/union_find/potential_union_find.hpp"

// The constraint merge(from, to, delta) means potential[to] - potential[from] = delta.
template <typename T = long long>
class WeightedUnionFind : public PotentialUnionFind<union_find_detail::AdditivePotentialGroup<T>> {
	using Base = PotentialUnionFind<union_find_detail::AdditivePotentialGroup<T>>;

       public:
	using Base::Base;
};
WeightedUnionFind(int) -> WeightedUnionFind<long long>;
