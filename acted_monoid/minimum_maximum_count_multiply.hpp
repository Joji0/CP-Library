#pragma once
#include "acted_monoid/detail/extrema_statistics.hpp"

// The aggregate exposes both extrema.
template <typename T = long long>
using MinimumMaximumCountMultiply = acted_monoid_detail::ExtremaStatistics<T, acted_monoid_detail::NumericUpdate::multiply, acted_monoid_detail::ExtremumQuery::both, false>;
