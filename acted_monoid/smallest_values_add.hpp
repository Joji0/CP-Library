#pragma once
#include "acted_monoid/detail/extreme_values_add.hpp"

template <typename T, int Capacity, bool Distinct = false>
using SmallestValuesAdd = acted_monoid_detail::ExtremeValuesAdd<T, Capacity, false, Distinct>;
