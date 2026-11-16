#pragma once
#include <vector>
#include "mathematics/polynomial/detail/polynomial_product_tree.hpp"

// 1-based point vector and results. Coefficient indices remain degrees.
// O(M(d) + M(n) log n), where d = max(degree, n).
template <typename Modular>
std::vector<Modular> polynomial_multipoint_evaluation(
    const FormalPowerSeries<Modular>& coefficients, const std::vector<Modular>& points) {
	return polynomial_detail::PolynomialProductTree<Modular>(points).values(coefficients);
}
