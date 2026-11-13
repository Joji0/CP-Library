#pragma once
#include "acted_monoid/detail/extrema_statistics.hpp"

// The aggregate exposes both extrema.
template <typename T = long long>
using MinimumMaximumCountAffine = acted_monoid_detail::ExtremaStatistics<T, acted_monoid_detail::NumericUpdate::affine, acted_monoid_detail::ExtremumQuery::both, false>;
