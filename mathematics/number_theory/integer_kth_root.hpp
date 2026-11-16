#pragma once
#include <bit>
#include <cstdint>
#include <stdexcept>

// floor(value^(1/degree)) for the full unsigned 64-bit domain.
inline uint64_t integer_kth_root(uint64_t value, unsigned degree) {
	if (degree == 0) throw std::invalid_argument("root degree must be positive");
	if (degree == 1 || value <= 1) return value;
	if (degree >= std::bit_width(value)) return 1;
	uint64_t low = 1, high = uint64_t(1) << ((std::bit_width(value) + degree - 1) / degree);
	while (low + 1 < high) {
		const uint64_t middle = low + (high - low) / 2;
		__uint128_t power = 1;
		for (unsigned i = 0; i < degree && power <= value; ++i) power *= middle;
		if (power <= value)
			low = middle;
		else
			high = middle;
	}
	return low;
}
