#pragma once
#include <utility>
#include "monoid/binary_inversions.hpp"
#include "monoid/detail/checked_arithmetic.hpp"

struct BinaryInversionsFlip : BinaryInversions {
	using Value = value_type;
	using Action = bool;

	static Value make_leaf(bool bit = false) { return BinaryInversions::make_leaf(bit); }
	static Value mapping(Action flip, Value value) {
		if (flip) {
			value.inversions = monoid_detail::checked_multiply_add(value.zeros, value.ones, monoid_detail::checked_subtract(0LL, value.inversions));
			std::swap(value.zeros, value.ones);
		}
		return value;
	}
	static Action composition(Action later, Action earlier) { return later != earlier; }
	static Action identity_action() { return false; }
};
