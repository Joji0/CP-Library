#pragma once
#include <algorithm>

struct MaxAffine {
	using S = long long;
	struct F {
		long long a, b;
	};
	static S op(S a, S b) { return std::max(a, b); }
	static S e() { return -(long long)1e18; }
	static S mapping(F f, S x) { return f.a * x + f.b; }
	static F composition(F f, F g) { return {f.a * g.a, f.a * g.b + f.b}; }
	static F id() { return {1, 0}; }
};
