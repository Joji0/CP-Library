#pragma once
#include "data_structure/union_find/detail/potential_groups.hpp"
#include "data_structure/union_find/rollback_potential_union_find.hpp"

template <typename T = long long>
using RollbackWeightedUnionFind = RollbackPotentialUnionFind<union_find_detail::AdditivePotentialGroup<T>>;
