#pragma once
#include "data_structure/union_find/detail/parity_constraints.hpp"
#include "data_structure/union_find/rollback_xor_union_find.hpp"

using RollbackParityUnionFind = union_find_detail::ParityConstraints<RollbackXorUnionFind<bool>>;
