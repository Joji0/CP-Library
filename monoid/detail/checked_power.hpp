#pragma once
#include <limits>
#include <type_traits>
#include "monoid/detail/checked_arithmetic.hpp"

namespace monoid_detail {
template <typename Number, typename Count>
Number multiply_by_power(Number value, Number factor, Count exponent) {
	while (exponent > 0) {
		if (exponent % 2) value = checked_multiply(value, factor);
		exponent /= 2;
		if (exponent > 0) factor = checked_multiply(factor, factor);
	}
	return value;
}

template <typename T, typename Count>
T checked_multiply_by_power(const T &value, const T &factor, Count exponent) {
	if (exponent == 0) return value;
	if (value == T(0) || factor == T(0)) return T(0);
	if constexpr (builtin_integer<T> && std::numeric_limits<T>::is_signed) {
		// A negative result can use one more magnitude than a positive one.
		const auto magnitude = multiply_by_power(unsigned_magnitude(value), unsigned_magnitude(factor), exponent);
		const bool negative = (value < 0) != (factor < 0 && exponent % 2);
		if (negative && magnitude == unsigned_magnitude(std::numeric_limits<T>::lowest())) {
			return std::numeric_limits<T>::lowest();
		}
		const T result = checked_count<T>(magnitude);
		return negative ? -result : result;
	} else {
		return multiply_by_power(value, factor, exponent);
	}
}
}  // namespace monoid_detail
