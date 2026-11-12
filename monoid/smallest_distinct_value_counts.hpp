#pragma once
#include "monoid/detail/distinct_value_counts.hpp"

// Frequencies of the Capacity smallest distinct values.
template <typename T, int Capacity>
using SmallestDistinctValueCounts = monoid_detail::DistinctValueCounts<T, Capacity, false>;
