#pragma once
#include "data_structure/union_find/detail/constraint_union_find_core.hpp"

// Records contradictions and tracks whether each component remains consistent.
template <typename Group>
using ConstraintUnionFind = union_find_detail::ConstraintUnionFindCore<Group, false>;
