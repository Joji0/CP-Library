#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <optional>
#include <random>
#include <utility>
#include <vector>
#include "acted_monoid/largest_distinct_value_counts_add_assign.hpp"
#include "acted_monoid/largest_distinct_value_counts_affine.hpp"
#include "acted_monoid/largest_distinct_value_counts_assign.hpp"
#include "acted_monoid/largest_distinct_value_counts_multiply.hpp"
#include "acted_monoid/largest_values_add_assign.hpp"
#include "acted_monoid/largest_values_affine.hpp"
#include "acted_monoid/largest_values_multiply.hpp"
#include "acted_monoid/smallest_distinct_value_counts_add_assign.hpp"
#include "acted_monoid/smallest_distinct_value_counts_affine.hpp"
#include "acted_monoid/smallest_distinct_value_counts_assign.hpp"
#include "acted_monoid/smallest_distinct_value_counts_multiply.hpp"
#include "acted_monoid/smallest_values_add_assign.hpp"
#include "acted_monoid/smallest_values_affine.hpp"
#include "acted_monoid/smallest_values_multiply.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
enum class Update { assign,
		    add_assign,
		    multiply,
		    affine };

template <typename Monoid, Update Operation, bool Largest, bool Distinct, bool Counts>
void check() {
	using Action = typename Monoid::Action;
	constexpr int size = 20;
	std::mt19937 random(4065);
	std::vector<long long> values(size + 1);
	for (int i = 1; i <= size; ++i) values[i] = int(random() % 15) - 7;
	LazySegmentTree<Monoid> tree(values);
	assert(!tree.query(2, 1).kth(1));
	assert(!tree.all_product().kth(0));

	for (int iteration = 0; iteration < 350; ++iteration) {
		const int left = iteration % 7 == 0 ? 1 : 1 + random() % size;
		const int right = iteration % 7 == 0 ? size : left + random() % (size - left + 1);
		const long long parameter = int(random() % 15) - 7;
		const long long slope = int(random() % 3) - 1;
		const bool set = (random() & 1) != 0;
		const auto [factor, offset] = [&]() -> std::pair<long long, long long> {
			if constexpr (Operation == Update::assign) return set ? std::pair{0LL, parameter} : std::pair{1LL, 0LL};
			if constexpr (Operation == Update::add_assign) return {set ? 0 : 1, parameter};
			if constexpr (Operation == Update::multiply) return {slope, 0};
			if constexpr (Operation == Update::affine) return {slope, parameter};
		}();
		const Action action = [&]() -> Action {
			if constexpr (Operation == Update::assign) return set ? Action(parameter) : std::nullopt;
			if constexpr (Operation == Update::add_assign) return set ? Monoid::make_set(parameter) : Monoid::make_add(parameter);
			if constexpr (Operation == Update::multiply) return slope;
			if constexpr (Operation == Update::affine) return {factor, offset};
		}();
		tree.update(left, right, action);
		for (int i = left; i <= right; ++i) values[i] = factor * values[i] + offset;
		const int position = 1 + random() % size;
		tree.set_value(position, parameter);
		values[position] = parameter;

		const int begin = iteration % 3 == 0 ? 1 : 1 + random() % size;
		const int end = iteration % 3 == 0 ? size : begin + random() % (size - begin + 1);
		const auto actual = tree.query(begin, end);
		assert(actual.count == end - begin + 1);
		std::vector<long long> sorted(values.begin() + begin, values.begin() + end + 1);
		if constexpr (Largest)
			std::sort(sorted.begin(), sorted.end(), std::greater<>{});
		else
			std::sort(sorted.begin(), sorted.end());
		auto ranks = sorted;
		if constexpr (Distinct) ranks.erase(std::unique(ranks.begin(), ranks.end()), ranks.end());
		for (int rank = 1; rank <= 4; ++rank) {
			const auto entry = actual.kth(rank);
			if (rank > 3 || static_cast<std::size_t>(rank) > ranks.size()) {
				assert(!entry);
			} else if constexpr (Counts) {
				assert(entry->value == ranks[rank - 1]);
				assert(entry->count == std::count(sorted.begin(), sorted.end(), entry->value));
			} else {
				assert(entry == ranks[rank - 1]);
			}
		}
	}
}

void check_boundaries() {
	using Assign = SmallestDistinctValueCountsAssign<long long, 1>;
	LazySegmentTree<Assign> tree(6);
	const auto lowest = std::numeric_limits<long long>::lowest();
	tree.update(1, 6, lowest);
	assert(tree.all_product().kth(1)->value == lowest);
	assert(tree.all_product().kth(1)->count == 6);
	assert(tree.query(3, 5).kth(1)->count == 3);
}
}  // namespace

int main() {
	check<SmallestValuesAddAssign<long long, 3>, Update::add_assign, false, false, false>();
	check<SmallestValuesAddAssign<long long, 3, true>, Update::add_assign, false, true, false>();
	check<SmallestValuesMultiply<long long, 3>, Update::multiply, false, false, false>();
	check<SmallestValuesMultiply<long long, 3, true>, Update::multiply, false, true, false>();
	check<SmallestValuesAffine<long long, 3>, Update::affine, false, false, false>();
	check<SmallestValuesAffine<long long, 3, true>, Update::affine, false, true, false>();
	check<SmallestDistinctValueCountsAddAssign<long long, 3>, Update::add_assign, false, true, true>();
	check<SmallestDistinctValueCountsMultiply<long long, 3>, Update::multiply, false, true, true>();
	check<SmallestDistinctValueCountsAffine<long long, 3>, Update::affine, false, true, true>();
	check<SmallestDistinctValueCountsAssign<long long, 3>, Update::assign, false, true, true>();
	check<LargestValuesAddAssign<long long, 3>, Update::add_assign, true, false, false>();
	check<LargestValuesAddAssign<long long, 3, true>, Update::add_assign, true, true, false>();
	check<LargestValuesMultiply<long long, 3>, Update::multiply, true, false, false>();
	check<LargestValuesMultiply<long long, 3, true>, Update::multiply, true, true, false>();
	check<LargestValuesAffine<long long, 3>, Update::affine, true, false, false>();
	check<LargestValuesAffine<long long, 3, true>, Update::affine, true, true, false>();
	check<LargestDistinctValueCountsAddAssign<long long, 3>, Update::add_assign, true, true, true>();
	check<LargestDistinctValueCountsMultiply<long long, 3>, Update::multiply, true, true, true>();
	check<LargestDistinctValueCountsAffine<long long, 3>, Update::affine, true, true, true>();
	check<LargestDistinctValueCountsAssign<long long, 3>, Update::assign, true, true, true>();
	check_boundaries();
	answer_library_checker_aplusb();
}
