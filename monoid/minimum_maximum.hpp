#pragma once
#include <algorithm>
#include <optional>

template <typename T = long long>
struct MinimumMaximum {
	struct Extrema {
		T minimum, maximum;
		bool operator==(const Extrema &) const = default;
	};
	using value_type = std::optional<Extrema>;

	static value_type make_leaf(const T &value) { return Extrema{value, value}; }
	static value_type identity() { return std::nullopt; }
	static value_type combine(value_type first, value_type second) {
		if (!first) return second;
		if (!second) return first;
		return Extrema{std::min(first->minimum, second->minimum), std::max(first->maximum, second->maximum)};
	}
};
