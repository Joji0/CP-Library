#pragma once
#include <cassert>
#include <utility>
#include <vector>
#include "data_structure/fenwick_tree/fenwick_tree.hpp"

template <typename T = long long>
class RangeFenwickTree {
	int n;
	FenwickTree<T> bit1, bit2;
	void add_prefix(int pos, T a, T b) {
		bit1.add(pos, a);
		bit2.add(pos, b);
	}

       public:
	RangeFenwickTree(int n = 0) { initialize(n); }
	explicit RangeFenwickTree(const std::vector<T> &values) { initialize(values); }

	void initialize(int n) {
		this->n = n;
		bit1.initialize(n);
		bit2.initialize(n);
	}

	// Builds in O(n) from values[1..n].
	void initialize(const std::vector<T> &values) {
		assert(!values.empty());
		n = int(values.size()) - 1;
		std::vector<T> differences(n + 1, T(0)), offsets(n + 1, T(0));
		T previous = T(0);
		for (int i = 1; i <= n; ++i) {
			const T difference = values[i] - previous;
			differences[i] = difference;
			offsets[i] = -difference * (i - 1);
			previous = values[i];
		}
		bit1.initialize(std::move(differences));
		bit2.initialize(std::move(offsets));
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	void range_add(int l, int r, T delta) {
		if (l > r) return;
		assert(1 <= l && r <= n);
		add_prefix(l, delta, -delta * (l - 1));
		if (r < n) add_prefix(r + 1, -delta, delta * r);
	}
	T prefix_sum(int r) const {
		assert(0 <= r && r <= n);
		if (r == 0) return T(0);
		return bit1.sum_prefix(r) * r + bit2.sum_prefix(r);
	}
	T range_sum(int l, int r) const {
		if (l > r) return T(0);
		assert(1 <= l && r <= n);
		return prefix_sum(r) - prefix_sum(l - 1);
	}
	T point_query(int pos) const { return get(pos); }
	T get(int pos) const { return range_sum(pos, pos); }
	void set(int pos, T value) { range_add(pos, pos, value - get(pos)); }
};
