#pragma once
#include <cstdint>
#include <algorithm>

struct MaxAffine {
	using S = int64_t;
	struct F {
		int64_t a, b;
	};
	static S op(S a, S b) { return std::max(a, b); }
	static S e() { return -(int64_t)1e18; }
	static S mapping(F f, S x) { return f.a * x + f.b; }
	static F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
	static F id() { return {1, 0}; }
};
