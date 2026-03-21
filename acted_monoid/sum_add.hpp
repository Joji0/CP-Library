#pragma once

struct SumAdd {
	struct S {
		long long val;
		int cnt;
	};
	using F = long long;
	static S op(S a, S b) { return {a.val + b.val, a.cnt + b.cnt}; }
	static S e() { return {0, 0}; }
	static S mapping(F f, S x) { return {x.val + f * x.cnt, x.cnt}; }
	static F composition(F f, F g) { return f + g; }
	static F id() { return 0; }
};
