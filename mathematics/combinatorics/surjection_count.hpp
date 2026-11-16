#pragma once
#include "mathematics/combinatorics.hpp"

// Number of functions from objects labeled elements onto targets labeled elements.
// O(targets * log(objects+1)) plus binomial queries.
template <typename Modular>
Modular surjection_count(long long objects, int targets, const Combinatorics<Modular>& combinations) {
	if (objects < 0 || targets < 0 || objects < targets) return 0;
	Modular result = 0;
	for (long long i = 0; i <= targets; ++i) {
		const Modular term = combinations.binomial_coefficient(targets, i) * Modular(targets - i).power(objects);
		result += (i & 1) ? -term : term;
	}
	return result;
}
