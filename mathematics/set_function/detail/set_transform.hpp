#pragma once
#include <bit>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace set_function_detail {

template <bool Superset, bool Inverse, typename Value>
void transform(std::vector<Value>& values) {
	if (!std::has_single_bit(values.size())) throw std::invalid_argument("expected one value per bitmask");
	for (std::size_t step = 1; step < values.size(); step *= 2) {
		for (std::size_t i = 0; i < values.size(); i += 2 * step) {
			for (std::size_t j = i; j < i + step; ++j) {
				const std::size_t source = Superset ? j + step : j;
				const std::size_t target = Superset ? j : j + step;
				if constexpr (Inverse)
					values[target] -= values[source];
				else
					values[target] += values[source];
			}
		}
	}
}

}  // namespace set_function_detail
