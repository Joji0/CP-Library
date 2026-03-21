#pragma once
#include <cstdint>
#include <algorithm>

struct MaxMonoid {
	using value_type = int64_t;
	static value_type e() { return -(int64_t)1e18; }
	static value_type op(value_type a, value_type b) { return std::max(a, b); }
};
