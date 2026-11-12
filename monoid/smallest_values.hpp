#pragma once
#include "monoid/detail/extreme_values.hpp"

// Keeps up to Capacity values in ascending order.
template <typename T, int Capacity>
using SmallestValues = monoid_detail::ExtremeValues<T, Capacity, false, false>;
