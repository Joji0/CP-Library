#pragma once
#include "acted_monoid/detail/power_sums_numeric_update.hpp"

// powers[d] is the sum of d-th powers for d >= 1. Degree zero is count.
// Mapping takes O(Degree log Degree). Intermediate coefficients must fit the checked arithmetic type.
template <typename T, std::size_t Degree>
using PowerSumsMultiply = acted_monoid_detail::PowerSumsNumericUpdate<T, Degree, acted_monoid_detail::NumericUpdate::multiply>;
