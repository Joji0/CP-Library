#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// Use extremum(value) for the selected bound. The aggregate also stores both bounds.
template <typename T = long long>
using SumMinimumAffine = acted_monoid_detail::SumNumericExtrema<T, acted_monoid_detail::NumericUpdate::affine, acted_monoid_detail::ExtremumQuery::minimum>;
