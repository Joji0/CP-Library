#pragma once
#include "data_structure/union_find/detail/potential_groups.hpp"
#include "data_structure/union_find/rollback_constraint_union_find.hpp"

template <typename T = long long>
using RollbackWeightedConstraintUnionFind = RollbackConstraintUnionFind<union_find_detail::AdditivePotentialGroup<T>>;
