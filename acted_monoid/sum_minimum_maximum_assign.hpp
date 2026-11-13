#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// The aggregate exposes both extrema.
template <typename T = long long>
using SumMinimumMaximumAssign = acted_monoid_detail::SumNumericExtrema<T, acted_monoid_detail::NumericUpdate::assign, acted_monoid_detail::ExtremumQuery::both>;
