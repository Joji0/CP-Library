#pragma once
#include "data_structure/union_find/detail/potential_groups.hpp"
#include "data_structure/union_find/dual_union_find.hpp"

template <typename T = long long>
using ComponentAddUnionFind = DualUnionFind<union_find_detail::AdditivePotentialGroup<T>>;
