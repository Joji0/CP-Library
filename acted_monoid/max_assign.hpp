#pragma once
#include <cstdint>
#include <algorithm>
#include <climits>

struct MaxAssign {
	using S = int64_t;
	using F = int64_t;
	static constexpr F NONE = LLONG_MIN;
	static S op(S a, S b) { return std::max(a, b); }
	static S e() { return -(int64_t)1e18; }
	static S mapping(F f, S x) { return f == NONE ? x : f; }
	static F composition(F f, F g) { return f == NONE ? g : f; }
	static F id() { return NONE; }
};
