#pragma once
#include <optional>
#include <type_traits>
#include "monoid/detail/checked_power.hpp"

namespace acted_monoid_detail {
template <typename T, bool Assign>
struct ProductNumericUpdate {
	using Action = std::conditional_t<Assign, std::optional<T>, T>;
	struct Value {
		T value = T(1);
		int count = 0;
		bool operator==(const Value &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(const Value &left, const Value &right) {
		return {monoid_detail::checked_multiply(left.value, right.value), monoid_detail::checked_add(left.count, right.count)};
	}
	static Value mapping(const Action &action, const Value &value) {
		if (value.count == 0) return value;
		if constexpr (Assign) {
			return action ? Value{monoid_detail::checked_multiply_by_power(T(1), *action, value.count), value.count} : value;
		} else {
			return {monoid_detail::checked_multiply_by_power(value.value, action, value.count), value.count};
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
