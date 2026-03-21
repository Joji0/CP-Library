#pragma once
#include <cstdint>
#include <climits>

struct SumAssign {
	struct S {
		int64_t val;
		int cnt;
	};
	using F = int64_t;
	static constexpr F NONE = LLONG_MIN;
	static S op(S a, S b) { return {a.val + b.val, a.cnt + b.cnt}; }
	static S e() { return {0, 0}; }
	static S mapping(F f, S x) { return f == NONE ? x : S{f * x.cnt, x.cnt}; }
	static F composition(F f, F g) { return f == NONE ? g : f; }
	static F id() { return NONE; }
};
