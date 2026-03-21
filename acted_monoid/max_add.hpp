#pragma once
#include <cstdint>
#include <algorithm>

struct MaxAdd {
	using S = int64_t;
	using F = int64_t;
	static S op(S a, S b) { return std::max(a, b); }
	static S e() { return -(int64_t)1e18; }
	static S mapping(F f, S x) { return x + f; }
	static F composition(F f, F g) { return f + g; }
	static F id() { return 0; }
};
