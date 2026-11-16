#pragma once
#include <stdexcept>
#include <utility>
#include <vector>
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Dense inverse of a sparse series modulo x^precision. Terms are
// {degree,coefficient}, sorted by strictly increasing degree. O(precision * terms.size()).
template <typename Modular>
FormalPowerSeries<Modular> sparse_formal_power_series_inverse(
    const std::vector<std::pair<int, Modular>>& terms, int precision) {
	if (precision < 0) throw std::invalid_argument("precision must be nonnegative");
	if (precision == 0) return {};
	if (terms.empty() || terms[0].first != 0) throw std::invalid_argument("inverse requires a unit constant");
	const Modular inverse = terms[0].second.inverse();
	for (std::size_t i = 1; i < terms.size(); ++i) {
		if (terms[i - 1].first >= terms[i].first) throw std::invalid_argument("degrees must strictly increase");
	}
	FormalPowerSeries<Modular> result(precision);
	result[0] = inverse;
	for (int i = 1; i < precision; ++i) {
		Modular sum = 0;
		for (std::size_t j = 1; j < terms.size() && terms[j].first <= i; ++j) {
			sum += result[i - terms[j].first] * terms[j].second;
		}
		result[i] = -sum * inverse;
	}
	return result;
}
