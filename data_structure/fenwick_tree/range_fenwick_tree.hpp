#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include "data_structure/fenwick_tree/fenwick_tree.hpp"

template <typename T = long long>
class RangeFenwickTree {
	using Storage = fenwick_tree_detail::Storage<T>;
	int n = 0;
	FenwickTree<Storage> differences, offsets;

	void add_range_value(int left, int right, const Storage &delta) {
		if (left > right) return;
		assert(1 <= left && right <= n);
		const Storage negative = Storage(0) - delta;
		differences.add(left, delta);
		offsets.add(left, negative * (left - 1));
		if (right < n) {
			differences.add(right + 1, negative);
			offsets.add(right + 1, delta * right);
		}
	}

	Storage prefix_value(int right) const {
		assert(0 <= right && right <= n);
		return differences.sum_prefix(right) * right + offsets.sum_prefix(right);
	}

	Storage range_value(int left, int right) const {
		if (left > right) return Storage(0);
		assert(1 <= left && right <= n);
		return prefix_value(right) - prefix_value(left - 1);
	}

       public:
	RangeFenwickTree(int size = 0) { initialize(size); }
	explicit RangeFenwickTree(const std::vector<T> &values) { initialize(values); }

	void initialize(int size) {
		FenwickTree<Storage> new_differences(size), new_offsets(size);
		differences = std::move(new_differences);
		offsets = std::move(new_offsets);
		n = size;
	}

	// Builds in O(n) from values[1..n].
	void initialize(const std::vector<T> &values) {
		assert(!values.empty());
		std::vector<Storage> delta(values.size(), Storage(0)), offset(values.size(), Storage(0));
		Storage previous(0);
		for (std::size_t i = 1; i < values.size(); ++i) {
			const Storage current(values[i]);
			delta[i] = current - previous;
			offset[i] = (Storage(0) - delta[i]) * int(i - 1);
			previous = current;
		}
		FenwickTree<Storage> new_differences(std::move(delta)), new_offsets(std::move(offset));
		differences = std::move(new_differences);
		offsets = std::move(new_offsets);
		n = int(values.size() - 1);
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	void range_add(int left, int right, T delta) { add_range_value(left, right, Storage(delta)); }
	void add(int pos, T delta) { range_add(pos, pos, delta); }
	void set(int pos, T value) { add_range_value(pos, pos, Storage(value) - range_value(pos, pos)); }
	T get(int pos) const { return range_sum(pos, pos); }
	T point_query(int pos) const { return get(pos); }

	T prefix_sum(int right) const { return fenwick_tree_detail::to_value<T>(prefix_value(right)); }
	T range_sum(int left, int right) const { return fenwick_tree_detail::to_value<T>(range_value(left, right)); }
	T sum(int left, int right) const { return range_sum(left, right); }
	T all_sum() const { return prefix_sum(n); }
};
