#pragma once
#include "acted_monoid/detail/bitwise_aggregate.hpp"

// Use product(value) to read the named bitwise query.
template <typename T = unsigned long long, std::size_t Bits = acted_monoid_detail::bit_width<T>>
using ExclusiveOrBitwiseNot = acted_monoid_detail::BitwiseAggregate<T, Bits, acted_monoid_detail::BitwiseUpdate::bitwise_not, acted_monoid_detail::BitwiseQuery::exclusive_or>;
