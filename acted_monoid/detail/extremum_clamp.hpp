#pragma once
#include <algorithm>
#include <optional>

namespace acted_monoid_detail {
template <typename T, bool Maximum, bool LowerBound>
struct ExtremumClamp {
	using Value = std::optional<T>;
	using Action = std::optional<T>;

	static Value identity() { return std::nullopt; }
	static Value make_leaf(const T &value = T(0)) { return value; }
	static Value combine(Value left, Value right) {
		if (!left) return right;
		if (!right) return left;
		if constexpr (Maximum)
			return std::max(*left, *right);
		else
			return std::min(*left, *right);
	}
	static Value mapping(Action action, Value value) {
		if (!action || !value) return value;
		if constexpr (LowerBound)
			return std::max(*value, *action);
		else
			return std::min(*value, *action);
	}
	static Action composition(Action later, Action earlier) {
		if (!earlier) return later;
		return mapping(later, earlier);
	}
	static Action identity_action() { return std::nullopt; }
};
}  // namespace acted_monoid_detail
