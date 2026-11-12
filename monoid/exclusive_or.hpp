#pragma once
#include <cstdint>

template <typename T = long long>
struct ExclusiveOr {
	using value_type = T;
	static value_type identity() { return 0; }
	static value_type combine(value_type a, value_type b) { return a ^ b; }
	static value_type remove_prefix(value_type a, value_type b) { return a ^ b; }
};
