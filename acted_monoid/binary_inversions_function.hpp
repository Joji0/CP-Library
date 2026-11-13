#pragma once
#include "acted_monoid/detail/binary_inversion_aggregate.hpp"
#include "acted_monoid/detail/binary_action.hpp"

using BinaryInversionsFunction = acted_monoid_detail::BinaryAction<acted_monoid_detail::BinaryInversionAggregate, acted_monoid_detail::BinaryUpdate::function>;
