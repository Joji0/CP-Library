#pragma once
#include <vector>
#include "mathematics/polynomial/detail/polynomial_product_tree.hpp"

// Points and values use dummy index 0. Points must be distinct over the field.
// Returns n coefficients, including trailing zeros. O(M(n) log n).
template <typename Modular>
FormalPowerSeries<Modular> polynomial_interpolation(
    const std::vector<Modular>& points, const std::vector<Modular>& values) {
	return polynomial_detail::PolynomialProductTree<Modular>(points).interpolate(values);
}
