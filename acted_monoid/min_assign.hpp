#pragma once
#include <algorithm>
#include <climits>

struct MinAssign {
	using S = long long;
	using F = long long;
	static constexpr F NONE = LLONG_MAX;
	static S op(S a, S b) { return std::min(a, b); }
	static S e() { return (long long)1e18; }
	static S mapping(F f, S x) { return f == NONE ? x : f; }
	static F composition(F f, F g) { return f == NONE ? g : f; }
	static F id() { return NONE; }
};
