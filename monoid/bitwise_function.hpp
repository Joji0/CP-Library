#pragma once
#include <type_traits>

// A truth table for each bit. combine(later, earlier) is later(earlier(x)).
template <typename T = unsigned long long>
struct BitwiseFunction {
	static_assert(std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<T, bool>);
	struct value_type {
		T from_zero, from_one;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {0, T(~T(0))}; }
	static value_type make_and(T mask) { return {0, mask}; }
	static value_type make_or(T mask) { return {mask, T(~T(0))}; }
	static value_type make_exclusive_or(T mask) { return {mask, T(~mask)}; }

	static T evaluate(value_type function, T input) {
		return T((T(~input) & function.from_zero) | (input & function.from_one));
	}
	static value_type combine(value_type later, value_type earlier) {
		return {evaluate(later, earlier.from_zero), evaluate(later, earlier.from_one)};
	}
};
