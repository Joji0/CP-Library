#pragma once
#include <type_traits>
#include "monoid/detail/checked_arithmetic.hpp"

namespace fenwick_tree_detail {
// Widen node sums, differences, and offsets before arithmetic on native integers.
template <typename T>
using Storage = std::conditional_t<monoid_detail::builtin_integer<T> && sizeof(T) <= sizeof(long long), __int128, T>;

template <typename T>
T to_value(const Storage<T> &value) {
	if constexpr (std::is_integral_v<T> && std::is_signed_v<T> && !std::is_same_v<T, Storage<T>>) {
		return monoid_detail::checked_count<T>(value);
	} else {
		return T(value);
	}
}
}  // namespace fenwick_tree_detail
