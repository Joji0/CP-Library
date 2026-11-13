#pragma once
#include <algorithm>
#include <cassert>
#include <optional>
#include <utility>
#include "acted_monoid/detail/numeric_extrema.hpp"
#include "monoid/detail/checked_arithmetic.hpp"

namespace acted_monoid_detail {
template <typename T, NumericUpdate Update, ExtremumQuery Query, bool Indexed>
struct ExtremaStatistics : NumericAction<T, Update> {
	static_assert(Update == NumericUpdate::assign || monoid_detail::builtin_integer<T>,
		      "Extremum counts and tie-breaking require injective arithmetic. Use an integral value type.");
	using Actions = NumericAction<T, Update>;
	using Action = typename Actions::Action;

	struct Extremum {
		T value{};
		int count = 0;
		int index = 0;
		bool operator==(const Extremum &) const = default;
	};
	struct Value {
		Extremum minimum, maximum;
		int count = 0;
		int first_index = 0;
		bool operator==(const Value &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(const T &value = T(0))
		requires(!Indexed)
	{
		const Extremum extremum{value, 1, 0};
		return {extremum, extremum, 1, 0};
	}
	static Value make_leaf(const T &value, int index)
		requires(Indexed)
	{
		assert(index >= 1);
		const Extremum extremum{value, 1, index};
		return {extremum, extremum, 1, index};
	}

	static Value combine(const Value &left, const Value &right) {
		if (left.count == 0) return right;
		if (right.count == 0) return left;
		return {choose<true>(left.minimum, right.minimum), choose<false>(left.maximum, right.maximum),
			monoid_detail::checked_add(left.count, right.count), std::min(left.first_index, right.first_index)};
	}
	static Value mapping(const Action &action, Value value) {
		if (value.count == 0) return value;
		const auto [factor, offset] = Actions::coefficients(action);
		if (factor == T(0)) {
			value.minimum = value.maximum = {offset, value.count, value.first_index};
		} else {
			value.minimum.value = monoid_detail::checked_multiply_add(factor, value.minimum.value, offset);
			value.maximum.value = monoid_detail::checked_multiply_add(factor, value.maximum.value, offset);
			if (factor < T(0)) std::swap(value.minimum, value.maximum);
		}
		return value;
	}
	static std::optional<Extremum> extremum(const Value &value)
		requires(Query != ExtremumQuery::both)
	{
		if (value.count == 0) return std::nullopt;
		if constexpr (Query == ExtremumQuery::minimum) return value.minimum;
		return value.maximum;
	}

       private:
	template <bool Minimum>
	static Extremum choose(const Extremum &left, const Extremum &right) {
		if (left.value == right.value) return {left.value, monoid_detail::checked_add(left.count, right.count), std::min(left.index, right.index)};
		if constexpr (Minimum) return left.value < right.value ? left : right;
		return right.value < left.value ? left : right;
	}
};
}  // namespace acted_monoid_detail
