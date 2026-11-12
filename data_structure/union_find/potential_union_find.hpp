#pragma once
#include "data_structure/union_find/detail/potential_union_find_core.hpp"

// Group supplies value_type, identity(), combine(a, b) = a * b and inverse().
template <typename Group>
using PotentialUnionFind = union_find_detail::PotentialUnionFindCore<Group, false>;
