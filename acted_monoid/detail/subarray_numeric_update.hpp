#pragma once
#include <optional>
#include "acted_monoid/detail/numeric_extrema.hpp"
#include "monoid/minimum_maximum_subarray_sum.hpp"

namespace acted_monoid_detail {
template <typename T, NumericUpdate Update, ExtremumQuery Query>
struct SubarrayNumericUpdate : MinimumMaximumSubarraySum<T>, NumericAction<T, Update> {
	static_assert(Update == NumericUpdate::assign || Update == NumericUpdate::multiply);
	using Base = MinimumMaximumSubarraySum<T>;
	using Actions = NumericAction<T, Update>;
	using Value = typename Base::value_type;
	using Action = typename Actions::Action;
	using Bounds = typename Base::Bounds;

	static Value mapping(const Action &action, const Value &value) {
		if (value.count == 0) return value;
		if constexpr (Update == NumericUpdate::assign) {
			return action ? Base::repeated(*action, value.count) : value;
		} else {
			const auto scale = [&](const Bounds &bounds) {
				const T first = monoid_detail::checked_multiply(action, bounds.minimum);
				const T second = monoid_detail::checked_multiply(action, bounds.maximum);
				return action < T(0) ? Bounds{second, first} : Bounds{first, second};
			};
			return {monoid_detail::checked_multiply(action, value.sum), scale(value.prefix), scale(value.suffix),
				scale(value.subarray), value.count};
		}
	}
	static std::optional<T> extremum(const Value &value)
		requires(Query != ExtremumQuery::both)
	{
		if (value.count == 0) return std::nullopt;
		if constexpr (Query == ExtremumQuery::minimum) return value.subarray.minimum;
		return value.subarray.maximum;
	}
};
}  // namespace acted_monoid_detail
