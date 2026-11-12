#pragma once
#include <cstdint>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct SumAddAssign {
	struct Value {
		T val;
		long long cnt;
	};
	static constexpr auto NONE = std::nullopt;
	struct Action {
		std::optional<T> set_val;
		T add_val;
		bool operator==(const Action &o) const { return set_val == o.set_val && add_val == o.add_val; }
	};

	static Value make_leaf(const T &value = T(0)) { return {value, 1}; }
	static Value combine(Value a, Value b) { return {monoid_detail::checked_add(a.val, b.val), monoid_detail::checked_add(a.cnt, b.cnt)}; }
	static Value identity() { return {0, 0}; }
	static Value mapping(Action f, Value x) {
		if (x.cnt == 0) return x;
		if (f.set_val) {
			return {monoid_detail::checked_scale(monoid_detail::checked_add(*f.set_val, f.add_val), x.cnt), x.cnt};
		}
		return {monoid_detail::checked_scaled_add(f.add_val, x.cnt, x.val), x.cnt};
	}
	static Action composition(Action f, Action g) {
		if (f.set_val) {
			return {f.set_val, f.add_val};
		}
		return {g.set_val, monoid_detail::checked_add(g.add_val, f.add_val)};
	}
	static Action identity_action() { return {NONE, 0}; }
	static Action make_set(T v) { return {v, 0}; }
	static Action make_add(T v) { return {NONE, v}; }
};
