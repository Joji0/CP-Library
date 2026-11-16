#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>

template <typename Unsigned = uint64_t, int HighestBit = std::numeric_limits<Unsigned>::digits - 1>
class ExclusiveOrBasis {
	static_assert(std::is_integral_v<Unsigned> && std::is_unsigned_v<Unsigned> && !std::is_same_v<Unsigned, bool> && sizeof(Unsigned) <= 8);
	static_assert(0 <= HighestBit && HighestBit < std::numeric_limits<Unsigned>::digits);
	std::array<Unsigned, HighestBit + 1> basis{};
	int rank = 0;

	static bool fits(Unsigned value) {
		if constexpr (HighestBit + 1 == std::numeric_limits<Unsigned>::digits)
			return true;
		else
			return (value >> (HighestBit + 1)) == 0;
	}

       public:
	// True if value adds a new independent direction. O(number of bits).
	bool add(Unsigned value) {
		if (!fits(value)) throw std::invalid_argument("input exceeds the configured bit width");
		for (int bit = HighestBit; bit >= 0; --bit) {
			if (((value >> bit) & 1) == 0) continue;
			if (basis[bit] == 0) {
				basis[bit] = value;
				++rank;
				return true;
			}
			value ^= basis[bit];
		}
		return false;
	}

	void merge(const ExclusiveOrBasis& other) {
		if (this == &other) return;
		for (Unsigned value : other.basis) {
			if (value != 0) add(value);
		}
	}

	bool can_represent(Unsigned value) const {
		if (!fits(value)) return false;
		for (int bit = HighestBit; bit >= 0; --bit) {
			if (((value >> bit) & 1) && basis[bit]) value ^= basis[bit];
		}
		return value == 0;
	}

	// Extremal value among seed XOR every element of the span.
	Unsigned maximum_exclusive_or(Unsigned seed = 0) const {
		for (int bit = HighestBit; bit >= 0; --bit) seed = std::max<Unsigned>(seed, seed ^ basis[bit]);
		return seed;
	}
	Unsigned minimum_exclusive_or(Unsigned seed = 0) const {
		for (int bit = HighestBit; bit >= 0; --bit) seed = std::min<Unsigned>(seed, seed ^ basis[bit]);
		return seed;
	}

	int size() const { return rank; }
	__uint128_t count_distinct() const { return __uint128_t(1) << rank; }

	// Order statistic in the distinct span, with rank 1 = 0. O(number of bits squared).
	std::optional<Unsigned> kth_smallest(__uint128_t position) const {
		if (position == 0 || position > count_distinct()) return std::nullopt;
		auto reduced = basis;
		Unsigned result = 0;
		--position;
		for (int bit = 0; bit <= HighestBit; ++bit) {
			if (reduced[bit] == 0) continue;
			for (int higher = bit + 1; higher <= HighestBit; ++higher) {
				if ((reduced[higher] >> bit) & 1) reduced[higher] ^= reduced[bit];
			}
			if (position & 1) result ^= reduced[bit];
			position >>= 1;
		}
		return result;
	}
};
