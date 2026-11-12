#pragma once
#include "acted_monoid/detail/extremum_clamp.hpp"

template <typename T = long long>
using MinimumUpperBound = acted_monoid_detail::ExtremumClamp<T, false, false>;
