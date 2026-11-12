#pragma once
#include "monoid/detail/distinct_value_counts.hpp"

// Frequencies of the Capacity largest distinct values.
template <typename T, int Capacity>
using LargestDistinctValueCounts = monoid_detail::DistinctValueCounts<T, Capacity, true>;
