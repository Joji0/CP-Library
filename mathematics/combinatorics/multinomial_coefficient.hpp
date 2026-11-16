#pragma once
#include <cassert>
#include <limits>
#include <stdexcept>
#include <vector>

// Number of rearrangements of counts[1], counts[2], ... repeated objects.
// Works with prime and composite binomial engines; slot 0 is ignored.
template <typename BinomialEngine>
auto multinomial_coefficient(const std::vector<long long>& counts, const BinomialEngine& engine) {
	assert(!counts.empty());
	auto result = engine.binomial_coefficient(0, 0);
	long long total = 0;
	for (std::size_t i = 1; i < counts.size(); ++i) {
		if (counts[i] < 0 || counts[i] > std::numeric_limits<long long>::max() - total) {
			throw std::invalid_argument("counts must be nonnegative and sum to a signed 64-bit integer");
		}
		total += counts[i];
		if constexpr (requires { engine.get_modulus(); }) {
			result = __int128(result) * engine.binomial_coefficient(total, counts[i]) % engine.get_modulus();
		} else {
			result *= engine.binomial_coefficient(total, counts[i]);
		}
	}
	return result;
}
