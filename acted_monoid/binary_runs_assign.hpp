#pragma once
#include "acted_monoid/detail/binary_run_aggregate.hpp"
#include "acted_monoid/detail/binary_action.hpp"

using BinaryRunsAssign = acted_monoid_detail::BinaryAction<acted_monoid_detail::BinaryRunAggregate, acted_monoid_detail::BinaryUpdate::assign>;
