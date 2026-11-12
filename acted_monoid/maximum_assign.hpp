#pragma once
#include <algorithm>
#include <optional>

template <typename T = long long>
struct MaximumAssign {
	using Value = std::optional<T>;
	using Action = std::optional<T>;
	static constexpr auto NONE = std::nullopt;

	static Value make_leaf(const T &value = T(0)) { return value; }
	static Value identity() { return std::nullopt; }
	static Value combine(Value first, Value second) {
		if (!first) return second;
		if (!second) return first;
		return std::max(*first, *second);
	}

	static Value mapping(Action action, Value value) {
		return value && action ? action : value;
	}
	static Action composition(Action later, Action earlier) { return later ? later : earlier; }
	static Action identity_action() { return std::nullopt; }
};
