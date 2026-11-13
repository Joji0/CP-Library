#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// The aggregate exposes both extrema.
template <typename T = long long>
using MinimumMaximumMultiply = acted_monoid_detail::NumericExtrema<T, acted_monoid_detail::NumericUpdate::multiply, acted_monoid_detail::ExtremumQuery::both>;
