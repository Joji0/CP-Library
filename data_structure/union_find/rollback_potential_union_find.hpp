#pragma once
#include "data_structure/union_find/detail/potential_union_find_core.hpp"

// Snapshots count successful structural merges. Queries do not compress paths.
template <typename Group>
using RollbackPotentialUnionFind = union_find_detail::PotentialUnionFindCore<Group, true>;
