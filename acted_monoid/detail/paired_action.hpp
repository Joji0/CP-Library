#pragma once
#include <type_traits>

namespace acted_monoid_detail {
template <typename First, typename Second>
struct PairedAction {
	using Action = typename First::Action;
	static_assert(std::is_same_v<Action, typename Second::Action>);
	struct Value {
		typename First::Value first;
		typename Second::Value second;
	};

	static Value identity() { return {First::identity(), Second::identity()}; }

	template <typename T = long long>
	static Value make_leaf(const T &value = T(0)) { return {First::make_leaf(value), Second::make_leaf(value)}; }

	static Value combine(const Value &left, const Value &right) {
		return {First::combine(left.first, right.first), Second::combine(left.second, right.second)};
	}
	static Value mapping(const Action &action, const Value &aggregate) {
		return {First::mapping(action, aggregate.first), Second::mapping(action, aggregate.second)};
	}
	static Action composition(const Action &later, const Action &earlier) { return First::composition(later, earlier); }
	static Action identity_action() { return First::identity_action(); }
};
}  // namespace acted_monoid_detail
