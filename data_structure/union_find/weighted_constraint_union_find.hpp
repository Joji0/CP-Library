#pragma once
#include "data_structure/union_find/constraint_union_find.hpp"
#include "data_structure/union_find/detail/potential_groups.hpp"

template <typename T = long long>
using WeightedConstraintUnionFind = ConstraintUnionFind<union_find_detail::AdditivePotentialGroup<T>>;
