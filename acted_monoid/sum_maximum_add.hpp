#pragma once
#include "acted_monoid/detail/paired_action.hpp"
#include "acted_monoid/maximum_add.hpp"
#include "acted_monoid/sum_add.hpp"

template <typename T = long long>
using SumMaximumAdd = acted_monoid_detail::PairedAction<SumAdd<T>, MaximumAdd<T>>;
