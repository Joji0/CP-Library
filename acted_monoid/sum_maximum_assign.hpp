#pragma once
#include "acted_monoid/detail/paired_action.hpp"
#include "acted_monoid/maximum_assign.hpp"
#include "acted_monoid/sum_assign.hpp"

template <typename T = long long>
using SumMaximumAssign = acted_monoid_detail::PairedAction<SumAssign<T>, MaximumAssign<T>>;
