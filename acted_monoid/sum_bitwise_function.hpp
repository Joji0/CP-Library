#pragma once
#include "acted_monoid/detail/bitwise_sum.hpp"

// Use sum(value) for a checked result or wide_sum(value) for an exact __int128 result.
template <typename T = long long, std::size_t Bits = acted_monoid_detail::bit_width<T>, typename Sum = acted_monoid_detail::DefaultBitwiseSum<T>>
using SumBitwiseFunction = acted_monoid_detail::BitwiseSum<T, Bits, Sum, acted_monoid_detail::BitwiseUpdate::function>;
