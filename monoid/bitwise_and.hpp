#pragma once
#include <cstdint>

template <typename T = long long>
struct BitwiseAnd {
	using value_type = T;
	static value_type identity() { return ~T(0); }
	static value_type combine(value_type a, value_type b) { return a & b; }
};
