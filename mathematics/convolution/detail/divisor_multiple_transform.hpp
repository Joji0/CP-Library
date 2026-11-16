#pragma once
#include <cassert>
#include <cstddef>
#include <vector>

namespace divisor_multiple_transform_detail {

template <bool Multiples, bool Inverse, typename Value>
void transform(std::vector<Value>& values) {
	assert(!values.empty());
	const std::size_t n = values.size() - 1;
	std::vector<bool> composite(n + 1);
	for (std::size_t prime = 2; prime <= n; ++prime) {
		if (composite[prime]) continue;
		if (prime <= n / prime) {
			for (std::size_t x = prime * prime; x <= n; x += prime) composite[x] = true;
		}
		auto update = [&](std::size_t i) {
			const std::size_t source = Multiples ? i * prime : i;
			const std::size_t target = Multiples ? i : i * prime;
			if constexpr (Inverse)
				values[target] -= values[source];
			else
				values[target] += values[source];
		};
		if constexpr (Multiples != Inverse) {
			for (std::size_t i = n / prime; i > 0; --i) update(i);
		} else {
			for (std::size_t i = 1; i <= n / prime; ++i) update(i);
		}
	}
}

}  // namespace divisor_multiple_transform_detail
