#pragma once
#include <optional>
#include "acted_monoid/minimum_maximum_add.hpp"
#include "monoid/detail/distinct_value_counts.hpp"

namespace acted_monoid_detail {
template <typename T, int Capacity, bool Largest>
struct DistinctValueCountsAdd {
	static_assert(monoid_detail::builtin_integer<T>,
		      "Distinct ranks require injective translation. Use an integral value type.");
	using Ordered = monoid_detail::DistinctValueCounts<T, Capacity, Largest>;
	using Bounds = MinimumMaximumAdd<T>;
	using Action = T;
	struct Value {
		typename Ordered::value_type ordered;
		typename Bounds::Value extrema;

		std::optional<typename Ordered::Entry> kth(int rank) const { return ordered.kth(rank); }
	};

	static Value identity() { return {Ordered::identity(), Bounds::identity()}; }
	static Value make_leaf(const T &value = T(0)) { return {Ordered::make_leaf(value), Bounds::make_leaf(value)}; }
	static Value combine(const Value &left, const Value &right) {
		return {Ordered::combine(left.ordered, right.ordered), Bounds::combine(left.extrema, right.extrema)};
	}
	static Value mapping(Action action, Value aggregate) {
		aggregate.extrema = Bounds::mapping(action, aggregate.extrema);
		for (int i = 0; i < aggregate.ordered.size; ++i) {
			auto &value = aggregate.ordered.entries[i + 1].value;
			value = monoid_detail::checked_add(value, action);
		}
		return aggregate;
	}
	static Action composition(Action later, Action earlier) { return Bounds::composition(later, earlier); }
	static Action identity_action() { return Bounds::identity_action(); }
};
}  // namespace acted_monoid_detail
