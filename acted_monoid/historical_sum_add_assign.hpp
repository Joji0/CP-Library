#pragma once
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_o
// Tracks current values and accumulated snapshots of those values. Range
// addition and assignment change only current values. An explicit accumulate
// action adds a multiple of the current values to the historical sum.
template <typename T = long long>
struct HistoricalSumAddAssign {
	struct Value {
		T current_sum{}, historical_sum{};
		long long length = 0;
		bool operator==(const Value &) const = default;
	};
	struct Action {
		bool preserves_current = true;
		T offset{}, history_scale{}, history_offset{};
		bool operator==(const Action &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(const T &current = T(0), const T &history = T(0)) { return {current, history, 1}; }
	static Value combine(const Value &left, const Value &right) {
		using namespace monoid_detail;
		return {checked_add(left.current_sum, right.current_sum), checked_add(left.historical_sum, right.historical_sum),
			checked_add(left.length, right.length)};
	}

	static Action identity_action() { return {}; }
	static Action make_add(const T &delta) { return {true, delta, T(0), T(0)}; }
	static Action make_assign(const T &value) { return {false, value, T(0), T(0)}; }
	static Action make_accumulate(const T &weight = T(1)) { return {true, T(0), weight, T(0)}; }

	static Value mapping(const Action &action, const Value &value) {
		if (value.length == 0) return value;
		using namespace monoid_detail;
		const T current = checked_scaled_add(action.offset, value.length, action.preserves_current ? value.current_sum : T(0));
		const T history = checked_add(value.historical_sum,
					      checked_linear_combination(action.history_scale, value.current_sum, action.history_offset, value.length));
		return {current, history, value.length};
	}
	static Action composition(const Action &later, const Action &earlier) {
		using namespace monoid_detail;
		return {later.preserves_current && earlier.preserves_current,
			later.preserves_current ? checked_add(earlier.offset, later.offset) : later.offset,
			earlier.preserves_current ? checked_add(earlier.history_scale, later.history_scale) : earlier.history_scale,
			checked_multiply_add(later.history_scale, earlier.offset, checked_add(earlier.history_offset, later.history_offset))};
	}
};
