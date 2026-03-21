#pragma once
#include <algorithm>

struct MaxAdd {
	using S = long long;
	using F = long long;
	static S op(S a, S b) { return std::max(a, b); }
	static S e() { return -(long long)1e18; }
	static S mapping(F f, S x) { return x + f; }
	static F composition(F f, F g) { return f + g; }
	static F id() { return 0; }
};
