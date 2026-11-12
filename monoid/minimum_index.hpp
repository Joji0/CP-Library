#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>

template <typename T = long long>
struct MinimumIndex {
	struct Node {
		T val;
		int idx;
	};
	using value_type = Node;
	static value_type identity() { return {std::numeric_limits<T>::max(), -1}; }
	static value_type combine(const value_type &a, const value_type &b) {
		if (a.idx == -1) return b;
		if (b.idx == -1) return a;
		if (a.val < b.val) return a;
		if (b.val < a.val) return b;
		return a.idx < b.idx ? a : b;
	}
};
