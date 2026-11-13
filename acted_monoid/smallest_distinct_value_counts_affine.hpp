#pragma once
#include "acted_monoid/detail/ranked_numeric_update.hpp"

// Query the aggregate with kth(one_based_rank). Each operation takes O(Capacity).
template <typename T, int Capacity>
using SmallestDistinctValueCountsAffine = acted_monoid_detail::RankedNumericUpdate<T, Capacity, false, true, true, acted_monoid_detail::NumericUpdate::affine>;
