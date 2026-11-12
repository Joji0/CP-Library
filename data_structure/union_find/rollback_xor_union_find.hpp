#pragma once
#include "data_structure/union_find/detail/potential_groups.hpp"
#include "data_structure/union_find/rollback_potential_union_find.hpp"

template <typename T = unsigned long long>
using RollbackXorUnionFind = RollbackPotentialUnionFind<union_find_detail::XorPotentialGroup<T>>;
