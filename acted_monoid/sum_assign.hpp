#pragma once
#include <cstdint>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct SumAssign {
	struct Value {
		T val;
		int cnt;
	};
	using Action = std::optional<T>;
	static constexpr auto NONE = std::nullopt;
	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(Value a, Value b) { return {monoid_detail::checked_add(a.val, b.val), monoid_detail::checked_add(a.cnt, b.cnt)}; }
	static Value identity() { return {0, 0}; }
	static Value mapping(Action f, Value x) { return f && x.cnt != 0 ? Value{monoid_detail::checked_scale(*f, x.cnt), x.cnt} : x; }
	static Action composition(Action f, Action g) { return f ? f : g; }
	static Action identity_action() { return NONE; }
};
