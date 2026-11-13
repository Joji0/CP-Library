#pragma once
#include "acted_monoid/detail/divisor_numeric_update.hpp"

// The aggregate value is an unsigned magnitude.
template <typename T = long long>
using LeastCommonMultipleAssign = acted_monoid_detail::DivisorNumericUpdate<T, true, true>;
