#pragma once
#include <algorithm>
#include <cstdint>
#include "mathematics/combinatorics.hpp"

// Number of ordered weak compositions of total into parts entries of [0,maximum].
// Uses inclusion-exclusion; O(min(parts,total/(maximum+1))) binomial queries.
template <typename Modular>
Modular bounded_composition_count(int total, int parts, int maximum, const Combinatorics<Modular>& combinations) {
	if (total < 0 || parts < 0 || maximum < 0) return 0;
	if (parts == 0) return total == 0 ? Modular(1) : Modular(0);
	const long long step = 1LL + maximum;
	if (total > 1LL * parts * maximum) return 0;
	Modular result = 0;
	for (long long j = 0; j <= std::min<long long>(parts, total / step); ++j) {
		const auto term = combinations.binomial_coefficient(parts, j) *
				  combinations.binomial_coefficient(total - j * step + parts - 1, parts - 1);
		result += (j & 1) ? -term : term;
	}
	return result;
}
