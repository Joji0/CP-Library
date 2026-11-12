#pragma once
#include <optional>
#include "acted_monoid/minimum_maximum_affine.hpp"
#include "monoid/detail/checked_arithmetic.hpp"
#include "monoid/detail/extreme_values.hpp"

namespace acted_monoid_detail {
template <typename T, int Capacity, bool Largest, bool Distinct>
struct ExtremeValuesAdd {
	static_assert(!Distinct || monoid_detail::builtin_integer<T>,
		      "Distinct ranks require injective translation. Use an integral value type.");
	using Ordered = monoid_detail::ExtremeValues<T, Capacity, Largest, Distinct>;
	using Bounds = MinimumMaximumAffine<T>;
	using Action = T;
	struct Value {
		typename Ordered::value_type ordered;
		typename Bounds::Value extrema;

		std::optional<T> kth(int rank) const { return ordered.kth(rank); }
	};

	static Value identity() { return {Ordered::identity(), Bounds::identity()}; }
	static Value make_leaf(const T &value = T(0)) { return {Ordered::make_leaf(value), Bounds::make_leaf(value)}; }
	static Value combine(const Value &left, const Value &right) {
		return {Ordered::combine(left.ordered, right.ordered), Bounds::combine(left.extrema, right.extrema)};
	}
	static Value mapping(Action action, Value aggregate) {
		// Check both ends, including values clipped out of the top-k list.
		aggregate.extrema = Bounds::mapping({T(1), action}, aggregate.extrema);
		for (int i = 0; i < aggregate.ordered.count; ++i) {
			auto &value = aggregate.ordered.values[i + 1];
			value = monoid_detail::checked_add(value, action);
		}
		return aggregate;
	}
	static Action composition(Action later, Action earlier) { return monoid_detail::checked_add(later, earlier); }
	static Action identity_action() { return T(0); }
};
}  // namespace acted_monoid_detail
