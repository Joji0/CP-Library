#pragma once
#include <cassert>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/convolution/convolution.hpp"

// Entry k counts permutations of size n with exactly k descents.
// Requires n+1 < prime modulus; O(M(n)+n log(n+1)).
template <typename Modular>
std::vector<Modular> eulerian_numbers(int n) {
	assert(n >= 0);
	if (n == 0) return {Modular(1)};
	Combinatorics<Modular> combinations(n + 1);
	std::vector<Modular> left(n), right(n);
	for (int i = 0; i < n; ++i) {
		left[i] = Modular(i + 1).power(n);
		auto count = combinations.binomial_coefficient(n + 1, i);
		right[i] = (i & 1) ? -count : count;
	}
	auto result = convolution(left, right);
	result.resize(n);
	return result;
}
