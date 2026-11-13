#pragma once
#include "acted_monoid/detail/subarray_numeric_update.hpp"

// Read nonempty subarray extrema from the aggregate. An empty range has count zero.
template <typename T = long long>
using MinimumSubarraySumMultiply = acted_monoid_detail::SubarrayNumericUpdate<T, acted_monoid_detail::NumericUpdate::multiply, acted_monoid_detail::ExtremumQuery::minimum>;
