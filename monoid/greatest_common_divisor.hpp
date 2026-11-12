#pragma once
#include <cstdint>
#include <numeric>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct GreatestCommonDivisor {
	using value_type = T;
	static value_type identity() { return 0; }
	static value_type combine(value_type a, value_type b) { return monoid_detail::checked_count<T>(std::gcd(monoid_detail::unsigned_magnitude(a), monoid_detail::unsigned_magnitude(b))); }
};
