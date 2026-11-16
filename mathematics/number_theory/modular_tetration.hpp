#pragma once
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include "mathematics/number_theory/euler_totient.hpp"

// Tower of 'height' copies of base (right-associative); height 0 and 0^0 both mean 1.
inline long long modular_tetration(long long base, uint64_t height, long long modulus) {
	if (base < 0 || modulus <= 0) throw std::invalid_argument("expected nonnegative base and positive modulus");
	if (base == 0) return (height & 1 ? 0 : 1) % modulus;
	if (base == 1) return 1 % modulus;

	// Return min(tower, limit). The next limit is a logarithm, so work is bounded
	// even when height is huge; intermediate products fit in unsigned 128 bits.
	auto capped = [&](auto&& self, uint64_t level, uint64_t limit) -> uint64_t {
		if (limit <= 1 || level == 0) return std::min(uint64_t(1), limit);
		__uint128_t value = 1;
		uint64_t threshold = 0;
		for (; value < limit; ++threshold) value *= base;
		const uint64_t exponent = self(self, level - 1, threshold);
		if (exponent >= threshold) return limit;
		value = 1;
		for (uint64_t i = 0; i < exponent; ++i) value *= base;
		return static_cast<uint64_t>(value);
	};
	auto evaluate = [&](auto&& self, uint64_t level, long long mod) -> long long {
		if (mod == 1) return 0;
		if (level == 0) return 1;
		if (level == 1) return base % mod;
		const uint64_t totient = euler_totient(mod);
		uint64_t exponent = self(self, level - 1, totient);
		if (capped(capped, level - 1, totient) == totient) exponent += totient;
		return NumberTheory::modular_power(base, exponent, mod);
	};
	return evaluate(evaluate, height, modulus);
}
