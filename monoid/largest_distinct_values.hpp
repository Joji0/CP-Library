#pragma once
#include "monoid/detail/extreme_values.hpp"

// Keeps up to Capacity distinct values in descending order.
template <typename T, int Capacity>
using LargestDistinctValues = monoid_detail::ExtremeValues<T, Capacity, true, true>;
