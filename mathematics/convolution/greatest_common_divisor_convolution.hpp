#pragma once
#include <stdexcept>
#include <vector>
#include "mathematics/convolution/multiple_mobius_transform.hpp"
#include "mathematics/convolution/multiple_zeta_transform.hpp"

// Result[k] = sum(a[i]*b[j]) over positive i,j with gcd(i,j) == k.
// Both vectors use indices 1..n and have the same size. Results outside 1..n
// are discarded. O(n log log n).
template <typename Value>
std::vector<Value> greatest_common_divisor_convolution(std::vector<Value> left, std::vector<Value> right) {
	if (left.empty() || left.size() != right.size()) throw std::invalid_argument("incompatible value tables");
	multiple_zeta_transform(left);
	multiple_zeta_transform(right);
	for (std::size_t i = 1; i < left.size(); ++i) left[i] *= right[i];
	multiple_mobius_transform(left);
	left[0] = Value(0);
	return left;
}
