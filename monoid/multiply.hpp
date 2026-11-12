#pragma once
#include <cstdint>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct Multiply {
	using value_type = T;
	static value_type identity() { return 1; }
	static value_type combine(value_type a, value_type b) { return monoid_detail::checked_multiply(a, b); }
	static value_type remove_prefix(value_type a, value_type b) { return monoid_detail::checked_divide(a, b); }
};
