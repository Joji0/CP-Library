#pragma once
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_n
// Range add on scores. Query the minimum score and the sum of the fixed
// weights of its minimizers. Weights can be zero, negative, or modular.
template <typename Score = long long, typename Weight = long long>
struct MinimumWeightSumAdd {
	struct Value {
		Score minimum{};
		Weight weight_sum{};
		bool empty = true;
		bool operator==(const Value &) const = default;
	};
	using Action = Score;

	static Value identity() { return {}; }
	static Value make_leaf(const Score &score, const Weight &weight) { return {score, weight, false}; }

	static Value combine(const Value &left, const Value &right) {
		if (left.empty) return right;
		if (right.empty) return left;
		if (left.minimum < right.minimum) return left;
		if (right.minimum < left.minimum) return right;
		return make_leaf(left.minimum, monoid_detail::checked_add(left.weight_sum, right.weight_sum));
	}
	static Value mapping(const Action &action, Value value) {
		if (!value.empty) value.minimum = monoid_detail::checked_add(value.minimum, action);
		return value;
	}
	static Action composition(const Action &later, const Action &earlier) { return monoid_detail::checked_add(later, earlier); }
	static Action identity_action() { return Score(0); }
};
