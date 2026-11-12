#pragma once
#include "monoid/detail/checked_arithmetic.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

// Concatenation hash over modular coefficients. Equal hashes can collide.
template <typename Modular = ModularInteger998244353, long long Base = 911382323>
struct PolynomialHash {
	struct value_type {
		Modular hash, power;
		long long length;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {0, 1, 0}; }
	static value_type make_leaf(const Modular &symbol) { return {symbol, Modular(Base), 1}; }
	static value_type combine(const value_type &left, const value_type &right) {
		return {monoid_detail::checked_multiply_add(left.hash, right.power, right.hash),
			monoid_detail::checked_multiply(left.power, right.power),
			monoid_detail::checked_add(left.length, right.length)};
	}
};
