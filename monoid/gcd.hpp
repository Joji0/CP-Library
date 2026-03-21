#pragma once
#include <numeric>

struct GcdMonoid {
	using value_type = long long;
	static value_type e() { return 0; }
	static value_type op(value_type a, value_type b) { return std::gcd(a, b); }
};
