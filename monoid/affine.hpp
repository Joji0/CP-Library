#pragma once
#include <cstdint>
#include "monoid/detail/checked_arithmetic.hpp"

// Monoid for composition of affine functions f(x) = a*x + b
template <typename T = long long>
struct Affine {
	struct Node {
		T a, b;
		bool operator==(const Node &o) const { return a == o.a && b == o.b; }
	};
	using value_type = Node;
	static value_type identity() { return {1, 0}; }
	// f op g = f(g(x)) = f.a * (g.a * x + g.b) + f.b = (f.a * g.a) * x + (f.a * g.b + f.b)
	static value_type combine(const value_type &f, const value_type &g) {
		return {monoid_detail::checked_multiply(f.a, g.a), monoid_detail::checked_multiply_add(f.a, g.b, f.b)};
	}
};
