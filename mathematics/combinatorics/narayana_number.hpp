#pragma once
#include <cassert>
#include <vector>
#include "mathematics/combinatorics.hpp"

// Entry k counts Dyck paths of semilength n with k peaks. O(n), prime modulus > n.
template <typename Modular>
std::vector<Modular> narayana_numbers(int n) {
	assert(n >= 0);
	if (n == 0) return {Modular(1)};
	assert(n < Modular::get_modulus());
	Combinatorics<Modular> combinations(n);
	std::vector<Modular> result(n + 1);
	const auto inverse = combinations.inverse(n);
	for (int k = 1; k <= n; ++k) {
		result[k] = combinations.binomial_coefficient(n, k) * combinations.binomial_coefficient(n, k - 1) * inverse;
	}
	return result;
}
