#pragma once
#include "acted_monoid/detail/paired_action.hpp"
#include "acted_monoid/minimum_add.hpp"
#include "acted_monoid/sum_add.hpp"

template <typename T = long long>
using SumMinimumAdd = acted_monoid_detail::PairedAction<SumAdd<T>, MinimumAdd<T>>;
