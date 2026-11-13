#pragma once
#include <optional>
#include "acted_monoid/detail/numeric_action.hpp"
#include "acted_monoid/minimum_maximum_affine.hpp"
#include "acted_monoid/sum_minimum_maximum_affine.hpp"

namespace acted_monoid_detail {
enum class ExtremumQuery { minimum,
			   maximum,
			   both };

template <typename T, NumericUpdate Update, ExtremumQuery Query>
struct NumericExtrema : MinimumMaximum<T>, NumericAction<T, Update> {
	using Bounds = MinimumMaximum<T>;
	using Actions = NumericAction<T, Update>;
	using Value = typename Bounds::value_type;
	using Action = typename Actions::Action;

	static Value make_leaf(const T &value = T(0)) { return Bounds::make_leaf(value); }
	static Value mapping(const Action &action, const Value &value) {
		if (!value) return value;
		const auto [factor, offset] = Actions::coefficients(action);
		return MinimumMaximumAffine<T>::mapping({factor, offset}, value);
	}
	static std::optional<T> extremum(const Value &value)
		requires(Query != ExtremumQuery::both)
	{
		if (!value) return std::nullopt;
		if constexpr (Query == ExtremumQuery::minimum) return value->minimum;
		return value->maximum;
	}
};

template <typename T, NumericUpdate Update, ExtremumQuery Query>
struct SumNumericExtrema : NumericAction<T, Update> {
	using Base = SumMinimumMaximumAffine<T>;
	using Actions = NumericAction<T, Update>;
	using Value = typename Base::Value;
	using Action = typename Actions::Action;

	static Value identity() { return Base::identity(); }
	static Value make_leaf(const T &value = T(0)) { return Base::make_leaf(value); }
	static Value combine(const Value &left, const Value &right) { return Base::combine(left, right); }
	static Value mapping(const Action &action, const Value &value) {
		if (!value.extrema) return value;
		return Base::mapping(Actions::coefficients(action), value);
	}
	static std::optional<T> extremum(const Value &value)
		requires(Query != ExtremumQuery::both)
	{
		return NumericExtrema<T, Update, Query>::extremum(value.extrema);
	}
};
}  // namespace acted_monoid_detail
