#pragma once
#include "data_structure/union_find/detail/potential_groups.hpp"
#include "data_structure/union_find/potential_union_find.hpp"

// Constraints are label[from] XOR label[to] = value.
template <typename T = unsigned long long>
using XorUnionFind = PotentialUnionFind<union_find_detail::XorPotentialGroup<T>>;
