#pragma once
#include <algorithm>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>

template <typename Integer = long long>
	requires(std::is_integral_v<Integer> && sizeof(Integer) <= 8)
std::optional<Integer> try_non_modular_binomial_coefficient(long long n, long long r) {
	if (r < 0 || r > n) return Integer(0);
	r = std::min(r, n - r);
	__uint128_t result = 1;
	for (long long i = 1; i <= r; ++i) {
		result = result * (n - r + i) / i;
		if (result > std::numeric_limits<Integer>::max()) return std::nullopt;
	}
	return static_cast<Integer>(result);
}

// Fixed-width inputs are checked. Big integer types supply their own arithmetic.
template <typename Integer = long long>
Integer non_modular_binomial_coefficient(long long n, long long r) {
	if constexpr (std::is_integral_v<Integer> && sizeof(Integer) <= 8) {
		auto result = try_non_modular_binomial_coefficient<Integer>(n, r);
		if (!result) throw std::overflow_error("binomial coefficient exceeds result type");
		return *result;
	} else {
		if (r < 0 || r > n) return Integer(0);
		r = std::min(r, n - r);
		Integer result = 1;
		for (long long i = 1; i <= r; ++i) result = result * (n - r + i) / i;
		return result;
	}
}
