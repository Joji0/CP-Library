#pragma once
#include "monoid/detail/checked_arithmetic.hpp"

struct BinaryInversions {
	struct value_type {
		long long zeros, ones, inversions;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {0, 0, 0}; }
	static value_type make_leaf(bool bit) { return {!bit, bit, 0}; }
	static value_type combine(const value_type &left, const value_type &right) {
		using namespace monoid_detail;
		return {checked_add(left.zeros, right.zeros), checked_add(left.ones, right.ones),
			checked_multiply_add(left.ones, right.zeros, checked_add(left.inversions, right.inversions))};
	}
};
