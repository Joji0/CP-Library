#pragma once
#include <optional>
#include "monoid/detail/extreme_values.hpp"

// Replaces each element by the assigned value, preserving the clipped multiplicity.
template <typename T, int Capacity, bool Distinct = false>
struct LargestValuesAssign : monoid_detail::ExtremeValues<T, Capacity, true, Distinct> {
	using Base = monoid_detail::ExtremeValues<T, Capacity, true, Distinct>;
	using Value = typename Base::value_type;
	using Action = std::optional<T>;

	static Value mapping(Action action, Value aggregate) {
		if (!action || aggregate.count == 0) return aggregate;
		if constexpr (Distinct) return Base::make_leaf(*action);
		for (int i = 0; i < aggregate.count; ++i) aggregate.values[i + 1] = *action;
		return aggregate;
	}
	static Action composition(Action later, Action earlier) { return later ? later : earlier; }
	static Action identity_action() { return std::nullopt; }
};
