#pragma once
#include <vector>
#include "mathematics/set_function/detail/set_transform.hpp"

// In-place subset zeta transform. Table index is a bitmask. O(k * 2^k).
template <typename Value>
void subset_zeta_transform(std::vector<Value>& values) {
	set_function_detail::transform<false, false>(values);
}
