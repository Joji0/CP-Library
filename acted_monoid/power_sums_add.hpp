#pragma once
#include "acted_monoid/detail/power_sums_numeric_update.hpp"

// powers[d] is the sum of d-th powers for d >= 1. Degree zero is count.
// Mapping takes O(Degree^2). Intermediate coefficients must fit the checked arithmetic type.
template <typename T, std::size_t Degree>
using PowerSumsAdd = acted_monoid_detail::PowerSumsNumericUpdate<T, Degree, acted_monoid_detail::NumericUpdate::add>;
