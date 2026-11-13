#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// Use extremum(value) for the selected bound. The aggregate also stores both bounds.
template <typename T = long long>
using SumMinimumMultiply = acted_monoid_detail::SumNumericExtrema<T, acted_monoid_detail::NumericUpdate::multiply, acted_monoid_detail::ExtremumQuery::minimum>;
