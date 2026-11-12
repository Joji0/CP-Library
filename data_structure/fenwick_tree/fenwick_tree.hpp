#pragma once
#include <cassert>
#include <bit>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <vector>
#include "data_structure/fenwick_tree/detail/arithmetic.hpp"

template <typename T = long long>
class FenwickTree {
	using Storage = fenwick_tree_detail::Storage<T>;
	int n = 0;
	std::vector<Storage> bit;

	void add_value(int position, const Storage &delta) {
		assert(1 <= position && position <= n);
		for (std::size_t i = position; i < bit.size(); i += i & -i) bit[i] += delta;
	}

	Storage prefix_value(int right) const {
		assert(0 <= right && right <= n);
		Storage result(0);
		for (int i = right; i > 0; i -= i & -i) result += bit[i];
		return result;
	}

	Storage range_value(int left, int right) const {
		if (left > right) return Storage(0);
		assert(1 <= left && right <= n);
		return prefix_value(right) - prefix_value(left - 1);
	}

	int search_prefix(Storage remaining, bool strict) const {
		const Storage zero(0);
		if (remaining < zero || (!strict && !(zero < remaining))) return 1;
		int position = 0;
		for (int step = int(std::bit_floor(static_cast<unsigned>(n))); step > 0; step >>= 1) {
			if (step > n - position) continue;
			const Storage &block = bit[position + step];
			if (strict ? !(remaining < block) : block < remaining) {
				position += step;
				remaining -= block;
			}
		}
		return position + 1;
	}

       public:
	FenwickTree(int size = 0) { initialize(size); }
	explicit FenwickTree(std::vector<T> values) { initialize(std::move(values)); }

	void initialize(int size) {
		assert(size >= 0);
		bit.assign(std::size_t(size) + 1, Storage(0));
		n = size;
	}

	// Builds in O(n) from values[1..n].
	void initialize(std::vector<T> values) {
		assert(!values.empty());
		const int size = int(values.size() - 1);
		std::vector<Storage> nodes;
		if constexpr (std::is_same_v<T, Storage>) {
			nodes = std::move(values);
		} else {
			nodes.assign(values.begin(), values.end());
		}
		nodes[0] = Storage(0);
		for (std::size_t i = 1; i < nodes.size(); ++i) {
			const auto parent = i + (i & -i);
			if (parent < nodes.size()) nodes[parent] += nodes[i];
		}
		bit = std::move(nodes);
		n = size;
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	void add(int pos, T delta) { add_value(pos, Storage(delta)); }
	void set(int pos, T value) { add_value(pos, Storage(value) - range_value(pos, pos)); }
	T get(int pos) const { return sum(pos, pos); }

	T sum_prefix(int right) const { return fenwick_tree_detail::to_value<T>(prefix_value(right)); }
	T sum(int left, int right) const { return fenwick_tree_detail::to_value<T>(range_value(left, right)); }
	T prefix_sum(int right) const { return sum_prefix(right); }
	T range_sum(int left, int right) const { return sum(left, right); }
	T all_sum() const { return sum_prefix(n); }

	// First 1-based prefix at least target. Requires nonnegative point values.
	int lower_bound(T target) const { return search_prefix(Storage(target), false); }

	// First 1-based prefix strictly greater than target, or size() + 1 if absent.
	// Both searches return size() + 1 when no prefix qualifies.
	int upper_bound(T target) const { return search_prefix(Storage(target), true); }
};
