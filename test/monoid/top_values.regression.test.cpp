#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <vector>
#include "acted_monoid/largest_distinct_value_counts_add.hpp"
#include "acted_monoid/largest_values_add.hpp"
#include "acted_monoid/largest_values_assign.hpp"
#include "acted_monoid/smallest_distinct_value_counts_add.hpp"
#include "acted_monoid/smallest_values_add.hpp"
#include "acted_monoid/smallest_values_assign.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/largest_distinct_value_counts.hpp"
#include "monoid/largest_distinct_values.hpp"
#include "monoid/largest_values.hpp"
#include "monoid/smallest_distinct_value_counts.hpp"
#include "monoid/smallest_distinct_values.hpp"
#include "monoid/smallest_values.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
template <typename Monoid, bool Largest, bool Distinct>
void compare(const std::vector<int> &values) {
	std::vector<typename Monoid::value_type> leaves(values.size(), Monoid::identity());
	for (std::size_t i = 1; i < values.size(); ++i) leaves[i] = Monoid::make_leaf(values[i]);
	SegmentTree<Monoid> tree(leaves);
	assert(!tree.query(1, 0).kth(1));
	for (int left = 1; left < int(values.size()); ++left) {
		for (int right = left; right < int(values.size()); ++right) {
			std::vector<int> sorted(values.begin() + left, values.begin() + right + 1);
			std::sort(sorted.begin(), sorted.end());
			if constexpr (Largest) std::reverse(sorted.begin(), sorted.end());
			if constexpr (Distinct) sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
			const auto actual = tree.query(left, right);
			for (int rank = 1; rank <= std::min(4, int(sorted.size())); ++rank) {
				assert(actual.kth(rank) == sorted[rank - 1]);
			}
			assert(!actual.kth(std::min(4, int(sorted.size())) + 1));
		}
	}
}

template <typename Monoid, bool Largest>
void compare_counts(const std::vector<int> &values) {
	auto aggregate = Monoid::identity();
	for (std::size_t i = 1; i < values.size(); ++i) aggregate = Monoid::combine(aggregate, Monoid::make_leaf(values[i]));
	std::vector<int> sorted(values.begin() + 1, values.end());
	std::sort(sorted.begin(), sorted.end());
	sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
	if constexpr (Largest) std::reverse(sorted.begin(), sorted.end());
	for (int rank = 1; rank <= std::min(4, int(sorted.size())); ++rank) {
		const auto actual = aggregate.kth(rank);
		assert(actual && actual->value == sorted[rank - 1]);
		assert(actual->count == std::count(values.begin() + 1, values.end(), actual->value));
	}
}

template <typename Monoid, bool Assign, bool Largest, bool Distinct>
void compare_updates(std::vector<int> values) {
	LazySegmentTree<Monoid> tree(values);
	std::mt19937 random(27521);
	for (int trial = 0; trial < 240; ++trial) {
		int left = 1 + random() % (values.size() - 1), right = 1 + random() % (values.size() - 1);
		if (left > right) std::swap(left, right);
		const int delta = int(random() % 11) - 5;
		tree.update(left, right, delta);
		for (int i = left; i <= right; ++i) values[i] = Assign ? delta : values[i] + delta;
		std::vector<int> expected(values.begin() + 1, values.end());
		std::sort(expected.begin(), expected.end());
		if constexpr (Largest) std::reverse(expected.begin(), expected.end());
		if constexpr (Distinct) expected.erase(std::unique(expected.begin(), expected.end()), expected.end());
		for (int rank = 1; rank <= std::min(4, int(expected.size())); ++rank) {
			assert(tree.all_product().kth(rank) == expected[rank - 1]);
		}
	}
}
}  // namespace

int main() {
	const std::vector<int> extremes{999, std::numeric_limits<int>::min(), -5, -5, 0, 7, 2,
					std::numeric_limits<int>::max(), 7, 2, 2, std::numeric_limits<int>::min()};
	compare<LargestValues<int, 4>, true, false>(extremes);
	compare<SmallestValues<int, 4>, false, false>(extremes);
	compare<LargestDistinctValues<int, 4>, true, true>(extremes);
	compare<SmallestDistinctValues<int, 4>, false, true>(extremes);
	compare_counts<LargestDistinctValueCounts<int, 4>, true>(extremes);
	compare_counts<SmallestDistinctValueCounts<int, 4>, false>(extremes);

	const std::vector<int> values{999, -6, -2, -2, -2, 0, 5, 5, 4, 3, 3, 4};
	compare_updates<LargestValuesAdd<int, 4>, false, true, false>(values);
	compare_updates<SmallestValuesAdd<int, 4, true>, false, false, true>(values);
	compare_updates<LargestValuesAssign<int, 4, true>, true, true, true>(values);
	compare_updates<SmallestValuesAssign<int, 4>, true, false, false>(values);
	const std::vector<int> repeated{999, -1, -1, 2, 2, 5};
	LazySegmentTree<LargestDistinctValueCountsAdd<int, 2>> largest(repeated);
	LazySegmentTree<SmallestDistinctValueCountsAdd<int, 2>> smallest(repeated);
	largest.update(1, 2, 3);
	smallest.update(1, 2, 3);
	assert(largest.all_product().kth(2)->value == 2 && largest.all_product().kth(2)->count == 4);
	assert(smallest.all_product().kth(1)->value == 2 && smallest.all_product().kth(1)->count == 4);
	answer_library_checker_aplusb();
}
