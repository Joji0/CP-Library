#pragma once
#include <cassert>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_v
// Range add on scores and range sum of history. Observing threshold t adds a
// weight to each element with score t. All observed scores must be >= t.
template <typename Score = long long, typename Weight = long long>
struct MinimumThresholdHistoryAdd {
	struct Value {
		Score minimum{};
		Weight historical_sum{};
		long long minimum_count = 0;
		bool operator==(const Value &) const = default;
	};
	struct Action {
		Score delta{};
		std::optional<Score> threshold;
		Weight weight{};
		bool operator==(const Action &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(const Score &score = Score(0), const Weight &history = Weight(0)) { return {score, history, 1}; }
	static Value combine(const Value &left, const Value &right) {
		using namespace monoid_detail;
		if (left.minimum_count == 0) return right;
		if (right.minimum_count == 0) return left;
		const Weight history = checked_add(left.historical_sum, right.historical_sum);
		if (left.minimum < right.minimum) return {left.minimum, history, left.minimum_count};
		if (right.minimum < left.minimum) return {right.minimum, history, right.minimum_count};
		return {left.minimum, history, checked_add(left.minimum_count, right.minimum_count)};
	}

	static Action identity_action() { return {}; }
	static Action make_add(const Score &delta) { return {delta, std::nullopt, Weight(0)}; }
	static Action make_observe(const Score &threshold, const Weight &weight = Weight(1)) { return {Score(0), threshold, weight}; }

	static Value mapping(const Action &action, Value value) {
		using namespace monoid_detail;
		if (value.minimum_count == 0) return value;
		if (action.threshold) {
			assert(!(value.minimum < *action.threshold));
			if (value.minimum == *action.threshold) {
				value.historical_sum = checked_scaled_add(action.weight, value.minimum_count, value.historical_sum);
			}
		}
		value.minimum = checked_add(value.minimum, action.delta);
		return value;
	}
	static Action composition(const Action &later, const Action &earlier) {
		using namespace monoid_detail;
		Action result{checked_add(earlier.delta, later.delta), earlier.threshold, earlier.weight};
		if (!later.threshold) return result;
		const Score required = checked_subtract(*later.threshold, earlier.delta);
		// For a legal starting score, observations below the largest required
		// threshold can never contribute. Equal thresholds add their weights.
		if (!result.threshold || *result.threshold < required) {
			result.threshold = required;
			result.weight = later.weight;
		} else if (*result.threshold == required) {
			result.weight = checked_add(result.weight, later.weight);
		}
		return result;
	}
};
