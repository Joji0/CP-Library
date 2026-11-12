#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

// powers[i] is the sum of i-th powers. Degree zero uses the element count.
template <typename T, std::size_t Degree>
struct PowerSumsAssign {
	struct Value {
		std::array<T, Degree + 1> powers{};
		long long count = 0;
		bool operator==(const Value &) const = default;
	};
	using Action = std::optional<T>;

	static Value identity() { return {}; }
	static Value make_leaf(const T &value = T(0)) {
		Value leaf;
		leaf.count = 1;
		return mapping(value, leaf);
	}
	static Value combine(const Value &left, const Value &right) {
		Value result;
		result.count = monoid_detail::checked_add(left.count, right.count);
		for (std::size_t i = 1; i <= Degree; ++i) {
			result.powers[i] = monoid_detail::checked_add(left.powers[i], right.powers[i]);
		}
		return result;
	}
	static Value mapping(Action action, Value aggregate) {
		if (!action || aggregate.count == 0) return aggregate;
		T power = T(1);
		for (std::size_t i = 1; i <= Degree; ++i) {
			power = monoid_detail::checked_multiply(power, *action);
			aggregate.powers[i] = monoid_detail::checked_scale(power, aggregate.count);
		}
		return aggregate;
	}
	static Action composition(Action later, Action earlier) { return later ? later : earlier; }
	static Action identity_action() { return std::nullopt; }
};
