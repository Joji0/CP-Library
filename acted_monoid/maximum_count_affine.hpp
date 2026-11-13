#pragma once
#include "acted_monoid/detail/extrema_statistics.hpp"

// Use extremum(value) for the selected bound. The aggregate also stores both bounds.
template <typename T = long long>
using MaximumCountAffine = acted_monoid_detail::ExtremaStatistics<T, acted_monoid_detail::NumericUpdate::affine, acted_monoid_detail::ExtremumQuery::maximum, false>;
