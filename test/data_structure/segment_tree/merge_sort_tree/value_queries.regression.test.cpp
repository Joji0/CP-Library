#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <limits>
#include <optional>
#include <random>
#include <vector>
#include "data_structure/segment_tree/merge_sort_tree.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	const MergeSortTree<long long> empty(std::vector<long long>{999});
	assert(empty.empty() && empty.size() == 0 && empty.count_equal(1, 0, 0) == 0);
	assert(!empty.try_predecessor(1, 0, 0) && !empty.try_successor(1, 0, 0));
	const MergeSortTree<long long> vacant(3);
	assert(vacant.size() == 3 && vacant.count_less_equal(1, 3, 0) == 0);
	assert(!vacant.try_predecessor(2, 3, 0) && !vacant.try_successor(1, 2, 0));

	constexpr long long lowest = std::numeric_limits<long long>::lowest();
	constexpr long long highest = std::numeric_limits<long long>::max();
	const MergeSortTree<long long> extremes(std::vector<long long>{999, lowest, highest, lowest});
	assert(extremes.count_equal(1, 3, lowest) == 2 && extremes.count_in_value_range(1, 3, lowest, highest) == 3);
	assert(extremes.try_predecessor(1, 3, lowest) == lowest && extremes.try_successor(1, 3, highest) == highest);
	assert(extremes.count_in_value_range(1, 3, highest, lowest) == 0);

	std::mt19937 random(603);
	for (int trial = 0; trial < 120; ++trial) {
		const int size = 1 + random() % 60;
		std::vector<long long> values(size + 1, 999);
		for (int position = 1; position <= size; ++position) values[position] = int(random() % 21) - 10;
		const MergeSortTree<long long> tree(values);
		for (int query = 0; query < 100; ++query) {
			int left = 1 + random() % size, right = 1 + random() % size;
			if (right < left) std::swap(left, right);
			const long long lower = int(random() % 27) - 13;
			const long long upper = int(random() % 27) - 13;
			int below = 0, equal = 0, above = 0, inside = 0;
			std::optional<long long> predecessor, successor;
			for (int i = left; i <= right; ++i) {
				const long long value = values[i];
				below += value < lower;
				equal += value == lower;
				above += lower < value;
				inside += lower <= value && value <= upper;
				if (value <= lower && (!predecessor || *predecessor < value)) predecessor = value;
				if (lower <= value && (!successor || value < *successor)) successor = value;
			}
			assert(tree.count_less(left, right, lower) == below);
			assert(tree.count_equal(left, right, lower) == equal);
			assert(tree.count_greater(left, right, lower) == above);
			assert(tree.count_less_equal(left, right, lower) == below + equal);
			assert(tree.count_greater_equal(left, right, lower) == above + equal);
			assert(tree.count_in_value_range(left, right, lower, upper) == inside);
			assert(tree.try_predecessor(left, right, lower) == predecessor);
			assert(tree.try_successor(left, right, lower) == successor);
		}
	}
	answer_library_checker_aplusb();
}
