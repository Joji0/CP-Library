#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

namespace monoid_detail {
template <typename T, std::size_t Size, bool Largest>
struct TropicalMatrix {
	static_assert(Size > 0);
	// A disengaged cell is unreachable. Matrix indices start at 1.
	using value_type = std::array<std::array<std::optional<T>, Size + 1>, Size + 1>;

	static value_type identity() {
		value_type result{};
		for (std::size_t i = 1; i <= Size; ++i) result[i][i] = T(0);
		return result;
	}

	static value_type combine(const value_type &left, const value_type &right) {
		value_type result{};
		for (std::size_t row = 1; row <= Size; ++row) {
			for (std::size_t middle = 1; middle <= Size; ++middle) {
				if (!left[row][middle]) continue;
				for (std::size_t column = 1; column <= Size; ++column) {
					if (!right[middle][column]) continue;
					const T candidate = checked_add(*left[row][middle], *right[middle][column]);
					auto &cell = result[row][column];
					if (!cell || (Largest ? *cell < candidate : candidate < *cell)) cell = candidate;
				}
			}
		}
		return result;
	}
};
}  // namespace monoid_detail
