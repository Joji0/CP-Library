#pragma once
#include <optional>
#include <type_traits>
#include "monoid/detail/checked_arithmetic.hpp"
#include "monoid/greatest_common_divisor.hpp"
#include "monoid/least_common_multiple.hpp"

namespace acted_monoid_detail {
template <typename T, bool Least, bool Assign>
struct DivisorNumericUpdate {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	using Magnitude = std::make_unsigned_t<T>;
	using Base = std::conditional_t<Least, LeastCommonMultiple<Magnitude>, GreatestCommonDivisor<Magnitude>>;
	// Multiplication affects GCD and LCM by the absolute magnitude of the factor.
	using Action = std::conditional_t<Assign, std::optional<T>, T>;
	struct Value {
		Magnitude value = Base::identity();
		bool empty = true;
		bool operator==(const Value &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(T value = T(0)) { return {monoid_detail::unsigned_magnitude(value), false}; }
	static Value combine(const Value &left, const Value &right) {
		return {Base::combine(left.value, right.value), left.empty && right.empty};
	}
	static Value mapping(const Action &action, const Value &value) {
		if (value.empty) return value;
		if constexpr (Assign) {
			return action ? make_leaf(*action) : value;
		} else {
			return {monoid_detail::checked_multiply(monoid_detail::unsigned_magnitude(action), value.value), false};
		}
	}
	static Action composition(const Action &later, const Action &earlier) {
		if constexpr (Assign) {
			return later ? later : earlier;
		} else {
			return monoid_detail::checked_multiply(later, earlier);
		}
	}
	static Action identity_action() {
		if constexpr (Assign)
			return std::nullopt;
		else
			return T(1);
	}
};
}  // namespace acted_monoid_detail
