#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = long long>
struct Minimum {
	using value_type = T;
	static value_type identity() {
		if constexpr (std::numeric_limits<T>::has_infinity) return std::numeric_limits<T>::infinity();
		return std::numeric_limits<T>::max();
	}
	static value_type combine(value_type a, value_type b) { return std::min(a, b); }
};
