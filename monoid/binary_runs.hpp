#pragma once
#include <algorithm>
#include <cassert>
#include "monoid/detail/checked_arithmetic.hpp"

struct BinaryRuns {
	struct Run {
		int prefix = 0;
		int suffix = 0;
		int longest = 0;
		int runs = 0;
		bool operator==(const Run &) const = default;
	};
	struct value_type {
		Run zero, one;
		int count = 0;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_leaf(bool bit = false) { return repeated(bit, 1); }
	static value_type combine(const value_type &left, const value_type &right) {
		const int count = monoid_detail::checked_add(left.count, right.count);
		return {join(left.zero, right.zero, left.count, right.count), join(left.one, right.one, left.count, right.count), count};
	}
	static value_type repeated(bool bit, int count) {
		assert(count >= 0);
		const Run run{count, count, count, count > 0};
		return bit ? value_type{{}, run, count} : value_type{run, {}, count};
	}

       private:
	static Run join(Run left, Run right, int left_count, int right_count) {
		return {left.prefix == left_count ? left_count + right.prefix : left.prefix,
			right.suffix == right_count ? right_count + left.suffix : right.suffix,
			std::max({left.longest, right.longest, left.suffix + right.prefix}),
			left.runs + right.runs - (left.suffix > 0 && right.prefix > 0)};
	}
};
