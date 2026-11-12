#pragma once
#include <array>
#include <vector>
#include "data_structure/frequency_summary/detail/misra_gries_merge.hpp"

// Capacity is chosen at construction. Nonempty summaries must have the same capacity.
template <int MaximumCapacity>
struct DynamicMisraGries {
	static_assert(MaximumCapacity >= 0);

	struct Node {
		std::array<int, MaximumCapacity> c{};
		std::array<long long, MaximumCapacity> v{};
		int k = 0;
		long long length = 0;

		Node() = default;
		Node(int value, int capacity, long long count = 1) : k(capacity), length(count) {
			if (capacity < 0 || capacity > MaximumCapacity || count < 0) monoid_detail::arithmetic_failure();
			if (capacity != 0) {
				c[0] = value;
				v[0] = count;
			}
		}

		int capacity() const {
			if (k < 0 || k > MaximumCapacity) monoid_detail::arithmetic_failure();
			return k;
		}

		std::vector<int> candidates() const {
			std::vector<int> result;
			const int active_capacity = capacity();
			for (int i = 0; i < active_capacity; ++i) {
				if (v[i] != 0) result.push_back(c[i]);
			}
			return result;
		}
	};

	using value_type = Node;
	static Node identity() { return {}; }

	static Node merge(Node left, const Node &right) {
		if (left.length == 0) return right;
		if (right.length == 0) return left;
		if (left.capacity() != right.capacity()) monoid_detail::arithmetic_failure();
		left.length = monoid_detail::checked_add(left.length, right.length);
		for (int i = 0; i < right.k; ++i) {
			frequency_summary_detail::offer(left.c, left.v, left.k, right.c[i], right.v[i]);
		}
		return left;
	}

	static Node combine(const Node &left, const Node &right) { return merge(left, right); }
};
