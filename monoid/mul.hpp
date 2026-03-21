#pragma once
#include <cstdint>

struct MulMonoid {
	using value_type = int64_t;
	static value_type e() { return 1; }
	static value_type op(value_type a, value_type b) { return a * b; }
};
