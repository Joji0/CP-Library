#pragma once
#include "acted_monoid/detail/extrema_statistics.hpp"

// Build indexed leaves with make_leaf(value, one_based_index). Ties use the smallest index.
template <typename T = long long>
using MaximumIndexAddAssign = acted_monoid_detail::ExtremaStatistics<T, acted_monoid_detail::NumericUpdate::add_assign, acted_monoid_detail::ExtremumQuery::maximum, true>;
