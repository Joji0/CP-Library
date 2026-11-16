#pragma once
#include <utility>
#include "mathematics/polynomial/polynomial_division.hpp"

// Euclidean algorithm over a field. Result is monic; gcd(0,0) is empty.
// Worst-case quadratic in degree. A fast half-gcd variant is a separate algorithm.
template <typename Modular>
FormalPowerSeries<Modular> polynomial_greatest_common_divisor(
    FormalPowerSeries<Modular> first, FormalPowerSeries<Modular> second) {
	first.shrink();
	second.shrink();
	while (!second.empty()) {
		auto remainder = polynomial_division(std::move(first), second).second;
		first = std::move(second);
		second = std::move(remainder);
	}
	if (!first.empty()) first /= first.back();
	return first;
}
