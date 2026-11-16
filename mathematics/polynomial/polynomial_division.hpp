#pragma once
#include <stdexcept>
#include <utility>
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Returns {quotient, remainder}, each trimmed. The divisor's leading coefficient must be a unit.
template <typename Modular>
std::pair<FormalPowerSeries<Modular>, FormalPowerSeries<Modular>> polynomial_division(
    FormalPowerSeries<Modular> dividend, FormalPowerSeries<Modular> divisor) {
	using Series = FormalPowerSeries<Modular>;
	dividend.shrink();
	divisor.shrink();
	if (divisor.empty()) throw std::invalid_argument("polynomial division by zero");
	if (dividend.size() < divisor.size()) return {{}, std::move(dividend)};
	const int count = int(dividend.size() - divisor.size() + 1);
	Series quotient;
	if (divisor.size() <= 32 || count <= 32) {
		quotient.resize(count);
		const auto inverse_leading = divisor.back().inverse();
		for (int i = count - 1; i >= 0; --i) {
			const auto coefficient = dividend[i + divisor.size() - 1] * inverse_leading;
			quotient[i] = coefficient;
			for (int j = 0; j < int(divisor.size()) - 1; ++j) dividend[i + j] -= coefficient * divisor[j];
		}
		dividend.resize(divisor.size() - 1);
	} else {
		quotient = (dividend.reversed().prefix(count) * divisor.reversed().inverse(count)).prefix(count).reversed();
		dividend -= quotient * divisor;
	}
	dividend.shrink();
	return {std::move(quotient), std::move(dividend)};
}
