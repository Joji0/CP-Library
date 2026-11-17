#pragma once
#include <cassert>
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_k
// Tracks sum and sum(i * values[i]) with 1-based positions within the aggregate.
template <typename T = long long>
struct PositionWeightedSum {
	struct value_type {
		long long length = 0;
		T sum{}, weighted_sum{};
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_leaf(const T &value) { return {1, value, value}; }

	static value_type make_repeated(const T &value, long long count) {
		using namespace monoid_detail;
		assert(count >= 0);
		if (count == 0 || value == T(0)) return {count, T(0), T(0)};
		const long long first = (count & 1) ? count : count / 2;
		const long long second = (count & 1) ? count / 2 + 1 : count + 1;
		const T partial = checked_scale(value, first);
		return {count, checked_scale(value, count), checked_scale(partial, second)};
	}

	static value_type combine(const value_type &left, const value_type &right) {
		using namespace monoid_detail;
		const T weighted_sum = [&] {
			if constexpr (builtin_integer<T> && sizeof(T) <= sizeof(long long)) {
				using Wide = std::conditional_t<std::numeric_limits<T>::is_signed, __int128, unsigned __int128>;
				return checked_count<T>(checked_add(checked_add(Wide(left.weighted_sum), Wide(right.weighted_sum)),
								    Wide(left.length) * Wide(right.sum)));
			} else {
				return checked_scaled_add(right.sum, left.length, checked_add(left.weighted_sum, right.weighted_sum));
			}
		}();
		return {checked_add(left.length, right.length), checked_add(left.sum, right.sum), weighted_sum};
	}
};
