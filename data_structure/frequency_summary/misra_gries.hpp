#pragma once
#include <array>
#include <vector>
#include "data_structure/frequency_summary/detail/misra_gries_merge.hpp"

// Mergeable candidate summary. A value with frequency > length / (Capacity + 1)
// is present among the candidates. Candidate sets can depend on merge order.
template <int Capacity>
struct MisraGries {
	static_assert(Capacity >= 0);

	struct Node {
		std::array<int, Capacity> c{};
		std::array<long long, Capacity> v{};
		long long length = 0;

		Node() = default;
		Node(int value, long long count = 1) : length(count) {
			if (count < 0) monoid_detail::arithmetic_failure();
			if constexpr (Capacity > 0) {
				c[0] = value;
				v[0] = count;
			}
		}

		static constexpr int capacity() { return Capacity; }

		std::vector<int> candidates() const {
			std::vector<int> result;
			for (int i = 0; i < Capacity; ++i) {
				if (v[i] != 0) result.push_back(c[i]);
			}
			return result;
		}
	};

	using value_type = Node;
	static Node identity() { return {}; }

	static Node merge(Node left, const Node &right) {
		left.length = monoid_detail::checked_add(left.length, right.length);
		for (int i = 0; i < Capacity; ++i) {
			frequency_summary_detail::offer(left.c, left.v, Capacity, right.c[i], right.v[i]);
		}
		return left;
	}

	// Retained for existing callers. The representation is not associative.
	static Node combine(const Node &left, const Node &right) { return merge(left, right); }
};
