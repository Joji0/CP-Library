#pragma once
#include <cstdint>
#include <numeric>
#include "monoid/detail/checked_arithmetic.hpp"

// The aggregate domain is nonnegative. Use make_leaf for raw signed values.
template <typename T = long long>
struct LeastCommonMultiple {
	using value_type = T;
	static value_type identity() { return 1; }
	static value_type make_leaf(value_type value) {
		return monoid_detail::checked_count<T>(monoid_detail::unsigned_magnitude(value));
	}
	static value_type combine(value_type a, value_type b) {
		if (a == 0 || b == 0) return 0;
		auto first = monoid_detail::unsigned_magnitude(a);
		const auto second = monoid_detail::unsigned_magnitude(b);
		first /= std::gcd(first, second);
		return monoid_detail::checked_count<T>(monoid_detail::checked_multiply(first, second));
	}
};
