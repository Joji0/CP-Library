#pragma once
#include <vector>
#include "mathematics/set_function/detail/set_transform.hpp"

// In-place subset mobius transform. Table index is a bitmask. O(k * 2^k).
template <typename Value>
void subset_mobius_transform(std::vector<Value>& values) {
	set_function_detail::transform<false, true>(values);
}
