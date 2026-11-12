#pragma once
#include "acted_monoid/detail/distinct_value_counts_add.hpp"

template <typename T, int Capacity>
using LargestDistinctValueCountsAdd = acted_monoid_detail::DistinctValueCountsAdd<T, Capacity, true>;
