#pragma once
#include "monoid/detail/extreme_values.hpp"

// Keeps up to Capacity distinct values in ascending order.
template <typename T, int Capacity>
using SmallestDistinctValues = monoid_detail::ExtremeValues<T, Capacity, false, true>;
