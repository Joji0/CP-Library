#pragma once
#include <cstdint>
#include <stdexcept>
#include <utility>

class BarrettReduction {
	uint32_t modulus;
	uint64_t reciprocal;

       public:
	explicit BarrettReduction(uint32_t modulus = 1) : modulus(modulus), reciprocal(0) {
		if (modulus == 0) throw std::invalid_argument("modulus must be positive");
		if (modulus > 1) reciprocal = UINT64_MAX / modulus + 1;
	}
	uint32_t get_modulus() const { return modulus; }
	// Supports the entire uint64_t input domain; estimated quotient is at most one too large.
	std::pair<uint64_t, uint32_t> divmod(uint64_t value) const {
		if (modulus == 1) return {value, 0};
		const uint64_t estimate = (__uint128_t(value) * reciprocal) >> 64;
		const __uint128_t product = __uint128_t(estimate) * modulus;
		if (product > value) return {estimate - 1, uint32_t(__uint128_t(value) + modulus - product)};
		return {estimate, uint32_t(value - product)};
	}
	uint32_t modulo(uint64_t value) const { return divmod(value).second; }
	uint64_t floor(uint64_t value) const { return divmod(value).first; }
	uint32_t multiply(uint32_t a, uint32_t b) const { return modulo(uint64_t(a) * b); }
};
