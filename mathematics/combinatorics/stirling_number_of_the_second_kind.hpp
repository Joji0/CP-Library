#pragma once
#include <cassert>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/convolution/convolution.hpp"

// Entry k counts partitions of n labeled objects into k nonempty unlabeled sets.
// Requires n < prime modulus; O(M(n) + n log(n+1)).
template <typename Modular>
std::vector<Modular> stirling_numbers_of_the_second_kind(int n) {
	assert(n >= 0);
	Combinatorics<Modular> combinations(n);
	std::vector<Modular> left(n + 1), right(n + 1);
	for (int i = 0; i <= n; ++i) {
		const auto inverse = combinations.inverse_factorial(i);
		left[i] = Modular(i).power(n) * inverse;
		right[i] = (i & 1) ? -inverse : inverse;
	}
	auto result = convolution(left, right);
	result.resize(n + 1);
	return result;
}
