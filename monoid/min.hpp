#pragma once
#include <algorithm>

struct MinMonoid {
	using value_type = long long;
	static value_type e() { return (long long)1e18; }
	static value_type op(value_type a, value_type b) { return std::min(a, b); }
};
