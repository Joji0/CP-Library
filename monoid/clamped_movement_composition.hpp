#pragma once
#include <algorithm>
#include <cassert>
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_q
// Concatenates moves x <- clamp(x + delta, lower, upper) and tracks the
// distance moved. Use the same clamp bounds for all leaves, and start inside them.
struct ClampedMovementComposition {
	struct value_type {
		long long first = 0, last = 0, offset = 0, peak_distance = 0;
		bool empty = true;
		bool operator==(const value_type &) const = default;
	};
	struct Movement {
		long long position;
		long long distance;
		bool operator==(const Movement &) const = default;
	};

	static value_type identity() { return {}; }

	static value_type make_shift(long long delta, long long lower, long long upper) {
		assert(lower <= upper);
		using monoid_detail::checked_count;
		return {checked_count<long long>(__int128(lower) - delta), checked_count<long long>(__int128(upper) - delta),
			delta, checked_count<long long>(delta >= 0 ? __int128(delta) : -__int128(delta)), false};
	}

	static Movement evaluate(const value_type &value, long long position) {
		if (value.empty) return {position, 0};
		const long long clamped = std::clamp(position, value.first, value.last);
		const __int128 lost = position < clamped ? __int128(clamped) - position : __int128(position) - clamped;
		return {monoid_detail::checked_add(clamped, value.offset),
			monoid_detail::checked_count<long long>(__int128(value.peak_distance) - lost)};
	}

	static value_type combine(const value_type &left, const value_type &right) {
		using monoid_detail::checked_count;
		if (left.empty) return right;
		if (right.empty) return left;
		const __int128 shifted_first = __int128(right.first) - left.offset;
		const __int128 shifted_last = __int128(right.last) - left.offset;
		const __int128 total_distance = __int128(left.peak_distance) + right.peak_distance;
		const __int128 first = std::max(__int128(left.first), shifted_first);
		const __int128 last = std::min(__int128(left.last), shifted_last);
		if (first <= last) {
			return {static_cast<long long>(first), static_cast<long long>(last),
				checked_count<long long>(__int128(left.offset) + right.offset),
				checked_count<long long>(total_distance), false};
		}
		const bool left_is_before = left.last < shifted_first;
		const long long anchor = left_is_before ? left.last : left.first;
		const long long target = left_is_before ? right.first : right.last;
		const __int128 gap = left_is_before ? shifted_first - left.last : left.first - shifted_last;
		return {anchor, anchor, checked_count<long long>(__int128(target) + right.offset - anchor),
			checked_count<long long>(total_distance - gap), false};
	}
};
