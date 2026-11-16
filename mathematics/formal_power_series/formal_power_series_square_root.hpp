#pragma once
#include <cassert>
#include <optional>
#include "mathematics/formal_power_series/formal_power_series.hpp"
#include "mathematics/number_theory/modular_square_root.hpp"

// One square root modulo x^precision over a prime field. nullopt means no root.
template <typename Modular>
std::optional<FormalPowerSeries<Modular>> formal_power_series_square_root(
    const FormalPowerSeries<Modular>& coefficients, int precision) {
	using Series = FormalPowerSeries<Modular>;
	assert(precision >= 0);
	const int available = std::min(precision, int(coefficients.size()));
	const long long modulus = Modular::get_modulus();
	assert(NumberTheory::is_prime(modulus));
	Series result(precision, Modular(0));
	if (modulus == 2) {
		for (int i = 0; i < available; ++i) {
			if ((i & 1) && coefficients[i] != Modular(0)) return std::nullopt;
			if (!(i & 1)) result[i / 2] = coefficients[i];
		}
		return result;
	}

	int first = 0;
	while (first < available && coefficients[first] == Modular(0)) ++first;
	if (first == available) return result;
	if (first & 1) return std::nullopt;
	const long long root = ModularSquareRoot::solve(coefficients[first].get(), modulus);
	if (root < 0) return std::nullopt;
	const int necessary = precision - first;
	const Modular inverse_two = Modular(2).inverse();
	Series tail{Modular(std::min(root, modulus - root))};
	for (int size = 1; size < necessary;) {
		const int next = size + std::min(size, necessary - size);
		Series input(coefficients.begin() + first, coefficients.begin() + first + std::min(next, available - first));
		tail = ((tail + input * tail.inverse(next)).prefix(next) * inverse_two);
		size = next;
	}
	std::copy(tail.begin(), tail.end(), result.begin() + first / 2);
	return result;
}
