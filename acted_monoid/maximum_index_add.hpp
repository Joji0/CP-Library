#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include "monoid/detail/checked_arithmetic.hpp"

template <typename T = long long>
struct MaximumIndexAdd {
	struct Value {
		T val;
		int idx;
	};
	using Action = T;
	static constexpr T NEG_INF = std::numeric_limits<T>::lowest();

	static Value make_leaf(const T &value, int index) {
		assert(index >= 1);
		return {value, index};
	}
	static Value combine(Value a, Value b) {
		if (a.idx == -1) return b;
		if (b.idx == -1) return a;
		if (a.val > b.val) return a;
		if (b.val > a.val) return b;
		return a.idx < b.idx ? a : b;
	}
	static Value identity() { return {NEG_INF, -1}; }
	static Value mapping(Action f, Value x) {
		if (x.idx == -1) return x;
		return {monoid_detail::checked_add(x.val, f), x.idx};
	}
	static Action composition(Action f, Action g) { return monoid_detail::checked_add(f, g); }
	static Action identity_action() { return 0; }
};
