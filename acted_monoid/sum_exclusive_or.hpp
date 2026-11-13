#pragma once
#include "acted_monoid/detail/bitwise_sum.hpp"

// Pass the XOR mask to update. Set Bits to the input width when it is known.
// Use sum(value) for a checked result or wide_sum(value) for an exact __int128 result.
template <typename T = long long, std::size_t Bits = acted_monoid_detail::bit_width<T>, typename Sum = acted_monoid_detail::DefaultBitwiseSum<T>>
using SumExclusiveOr = acted_monoid_detail::BitwiseSum<T, Bits, Sum, acted_monoid_detail::BitwiseUpdate::exclusive_or>;
