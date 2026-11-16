#pragma once
#include "mathematics/convolution/detail/divisor_multiple_transform.hpp"

// In place over positive integer indices 1..n. Slot 0 is ignored. O(n log log n).
template <typename Value>
void divisor_zeta_transform(std::vector<Value>& values) {
	divisor_multiple_transform_detail::transform<false, false>(values);
}
