#pragma once
#include "acted_monoid/detail/numeric_extrema.hpp"

// Use extremum(value) for the selected bound. The aggregate also stores both bounds.
template <typename T = long long>
using MinimumAddAssign = acted_monoid_detail::NumericExtrema<T, acted_monoid_detail::NumericUpdate::add_assign, acted_monoid_detail::ExtremumQuery::minimum>;
