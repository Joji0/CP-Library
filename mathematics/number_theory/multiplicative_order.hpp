#pragma once
#include <numeric>
#include <optional>
#include "mathematics/number_theory/euler_totient.hpp"

// Smallest positive exponent producing 1 mod modulus. nullopt for nonunits or modulus <= 1.
inline std::optional<long long> multiplicative_order(long long value, long long modulus) {
	if (modulus <= 1) return std::nullopt;
	value = NumberTheory::safe_modulo(value, modulus);
	if (std::gcd(value, modulus) != 1) return std::nullopt;
	long long order = euler_totient(modulus);
	for (auto prime : NumberTheory::factorize(order)) {
		if (order % prime == 0 && NumberTheory::modular_power(value, order / prime, modulus) == 1) order /= prime;
	}
	return order;
}
