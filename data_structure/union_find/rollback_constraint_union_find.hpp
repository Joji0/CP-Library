#pragma once
#include "data_structure/union_find/detail/constraint_union_find_core.hpp"

template <typename Group>
using RollbackConstraintUnionFind = union_find_detail::ConstraintUnionFindCore<Group, true>;
