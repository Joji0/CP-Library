#pragma once
#include <optional>

// Last assignment wins in left-to-right sequence order.
template <typename T>
struct Assignment {
	using value_type = std::optional<T>;
	static value_type identity() { return std::nullopt; }
	static value_type make_leaf(const T &value) { return value; }
	static value_type combine(const value_type &earlier, const value_type &later) {
		return later ? later : earlier;
	}
	static value_type composition(const value_type &later, const value_type &earlier) {
		return combine(earlier, later);
	}
};
