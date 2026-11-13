#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// The aggregate exposes both extrema.
template <typename T = long long>
using MinimumMaximumAddAssign = acted_monoid_detail::NumericExtrema<T, acted_monoid_detail::NumericUpdate::add_assign, acted_monoid_detail::ExtremumQuery::both>;
