#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace monoid_detail {
[[noreturn]] inline void arithmetic_failure() noexcept { std::abort(); }

template <typename T>
inline constexpr bool builtin_integer = std::is_integral_v<T> || std::is_same_v<T, __int128> ||
					std::is_same_v<T, unsigned __int128>;

template <typename T>
constexpr T checked_finite(T value) {
	if constexpr (std::is_floating_point_v<T>) {
		if (!std::isfinite(value)) arithmetic_failure();
	}
	return value;
}

template <typename T>
constexpr T checked_add(const T &left, const T &right) {
	if constexpr (std::is_same_v<T, bool>) {
		if (left && right) arithmetic_failure();
		return left || right;
	} else if constexpr (builtin_integer<T>) {
		T result;
		if (__builtin_add_overflow(left, right, &result)) arithmetic_failure();
		return result;
	} else {
		return checked_finite(T(left + right));
	}
}

template <typename T>
constexpr T checked_subtract(const T &left, const T &right) {
	if constexpr (std::is_same_v<T, bool>) {
		if (!left && right) arithmetic_failure();
		return left && !right;
	} else if constexpr (builtin_integer<T>) {
		T result;
		if (__builtin_sub_overflow(left, right, &result)) arithmetic_failure();
		return result;
	} else {
		return checked_finite(T(left - right));
	}
}

template <typename T>
constexpr T checked_multiply(const T &left, const T &right) {
	if constexpr (builtin_integer<T> && !std::is_same_v<T, bool>) {
		T result;
		if (__builtin_mul_overflow(left, right, &result)) arithmetic_failure();
		return result;
	} else {
		return checked_finite(T(left * right));
	}
}

template <typename T>
constexpr T checked_divide(const T &left, const T &right) {
	if constexpr (builtin_integer<T>) {
		if (right == 0) arithmetic_failure();
		if constexpr (std::numeric_limits<T>::is_signed) {
			if (left == std::numeric_limits<T>::lowest() && right == T(-1)) arithmetic_failure();
		}
	}
	return checked_finite(T(left / right));
}

template <typename T>
constexpr T checked_multiply_add(const T &left, const T &right, const T &offset) {
	if constexpr (builtin_integer<T> && sizeof(T) <= sizeof(long long)) {
		using Wide = std::conditional_t<std::numeric_limits<T>::is_signed, __int128, unsigned __int128>;
		const Wide result = Wide(left) * Wide(right) + Wide(offset);
		if (result < Wide(std::numeric_limits<T>::lowest()) || result > Wide(std::numeric_limits<T>::max())) {
			arithmetic_failure();
		}
		return T(result);
	} else {
		return checked_add(checked_multiply(left, right), offset);
	}
}

template <typename T, typename Count>
constexpr T checked_count(Count count) {
	if constexpr (builtin_integer<T>) {
		// Adding zero converts via the overflow builtin without an intermediate narrowing cast.
		T result;
		if constexpr (std::is_same_v<T, bool>) {
			if (count != 0 && count != 1) arithmetic_failure();
			return T(count);
		} else {
			if (__builtin_add_overflow(count, 0, &result)) arithmetic_failure();
			return result;
		}
	} else {
		return T(count);
	}
}

template <typename T, typename Count>
constexpr T checked_linear_combination(const T &first, const T &second, const T &scale, Count count) {
	if constexpr (builtin_integer<T> && sizeof(T) <= sizeof(long long)) {
		using Wide = std::conditional_t<std::numeric_limits<T>::is_signed, __int128, unsigned __int128>;
		const Wide left = Wide(first) * Wide(second);
		const Wide right = Wide(scale) * Wide(count);
		return checked_count<T>(checked_add(left, right));
	} else {
		return checked_multiply_add(first, second, checked_multiply(scale, checked_count<T>(count)));
	}
}

template <typename T, typename Count>
constexpr T checked_scaled_add(const T &value, Count count, const T &offset) {
	return checked_linear_combination(T(1), offset, value, count);
}

template <typename T, typename Count>
constexpr T checked_scale(const T &value, Count count) {
	return checked_linear_combination(T(0), T(0), value, count);
}

template <typename T>
constexpr auto unsigned_magnitude(T value) {
	using Unsigned = std::make_unsigned_t<T>;
	const Unsigned converted = static_cast<Unsigned>(value);
	if constexpr (std::numeric_limits<T>::is_signed) {
		if (value < 0) return Unsigned(Unsigned(0) - converted);
	}
	return converted;
}
}  // namespace monoid_detail
