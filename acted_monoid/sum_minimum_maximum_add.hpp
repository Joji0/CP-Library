#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// The aggregate exposes both extrema.
template <typename T = long long>
using SumMinimumMaximumAdd = acted_monoid_detail::SumNumericExtrema<T, acted_monoid_detail::NumericUpdate::add, acted_monoid_detail::ExtremumQuery::both>;
