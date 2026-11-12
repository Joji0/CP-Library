#pragma once
#include "data_structure/union_find/detail/parity_constraints.hpp"
#include "data_structure/union_find/xor_union_find.hpp"

using ParityUnionFind = union_find_detail::ParityConstraints<XorUnionFind<bool>>;
