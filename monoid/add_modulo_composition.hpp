#pragma once
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_h
// Composition in sequence order: combine(left,right) applies left first.
// A nontrivial reduction stores (x + before) mod Modulus + after.
template <long long Modulus>
struct AddModuloComposition {
	static_assert(Modulus > 0);
	struct value_type {
		long long before = 0, after = 0;
		bool reduces = false;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_add(long long amount) { return {amount, 0, false}; }
	static value_type make_modulo() { return {0, 0, true}; }

	static value_type combine(const value_type &left, const value_type &right) {
		using namespace monoid_detail;
		if (right.reduces) return {reduce(__int128(left.before) + left.after + right.before), right.after, true};
		if (left.reduces) return {left.before, checked_add(left.after, right.before), true};
		return make_add(checked_add(left.before, right.before));
	}

	static long long evaluate(const value_type &function, long long input) {
		using namespace monoid_detail;
		if (!function.reduces) return checked_add(input, function.before);
		return checked_add(reduce(__int128(input) + function.before), function.after);
	}

       private:
	static long long reduce(__int128 value) {
		long long remainder = static_cast<long long>(value % Modulus);
		return remainder < 0 ? remainder + Modulus : remainder;
	}
};
