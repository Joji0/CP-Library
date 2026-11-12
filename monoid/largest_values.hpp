#pragma once
#include "monoid/detail/extreme_values.hpp"

// Keeps up to Capacity values in descending order.
template <typename T, int Capacity>
using LargestValues = monoid_detail::ExtremeValues<T, Capacity, true, false>;
