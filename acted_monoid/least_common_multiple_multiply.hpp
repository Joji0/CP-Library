#pragma once
#include "acted_monoid/detail/divisor_numeric_update.hpp"

// The aggregate value is an unsigned magnitude. Multiplication uses the absolute value of the factor.
template <typename T = long long>
using LeastCommonMultipleMultiply = acted_monoid_detail::DivisorNumericUpdate<T, true, false>;
