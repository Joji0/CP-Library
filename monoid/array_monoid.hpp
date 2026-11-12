#pragma once
#include <array>
#include <cstddef>

// Combines components independently. Component indices are 1-based.
template <typename Monoid, std::size_t Size>
struct ArrayMonoid {
	using value_type = std::array<typename Monoid::value_type, Size + 1>;
	static value_type identity() {
		value_type result;
		result.fill(Monoid::identity());
		return result;
	}
	static value_type combine(const value_type &left, const value_type &right) {
		value_type result;
		result[0] = Monoid::identity();
		for (std::size_t i = 1; i <= Size; ++i) result[i] = Monoid::combine(left[i], right[i]);
		return result;
	}
};
