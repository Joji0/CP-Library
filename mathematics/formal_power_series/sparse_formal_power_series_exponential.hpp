#pragma once
#include <stdexcept>
#include <utility>
#include <vector>
#include "mathematics/combinatorics.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Dense exponential of a sparse series modulo x^precision. List only positive
// distinct degrees in ascending order. O(precision * (1 + terms.size())).
template <typename Modular>
FormalPowerSeries<Modular> sparse_formal_power_series_exponential(
    const std::vector<std::pair<int, Modular>>& terms, int precision) {
	if (precision < 0) throw std::invalid_argument("precision must be nonnegative");
	if (!terms.empty() && terms[0].first <= 0) throw std::invalid_argument("exponential requires zero constant");
	if (precision == 0) return {};
	Combinatorics<Modular> combinations(precision - 1);
	for (std::size_t i = 1; i < terms.size(); ++i) {
		if (terms[i - 1].first >= terms[i].first) throw std::invalid_argument("degrees must strictly increase");
	}
	FormalPowerSeries<Modular> result(precision);
	result[0] = 1;
	auto derivative = terms;
	for (auto& [degree, coefficient] : derivative) coefficient *= Modular(degree);
	for (int i = 1; i < precision; ++i) {
		Modular sum = 0;
		for (const auto& [degree, coefficient] : derivative) {
			if (degree > i) break;
			sum += result[i - degree] * coefficient;
		}
		result[i] = sum * combinations.factorial(i - 1) * combinations.inverse_factorial(i);
	}
	return result;
}
