#include <cassert>
#include <iterator>
#include <limits>
#include <optional>
#include <random>
#include <set>
#include <utility>
#include "data_structure/treap/treap.hpp"

namespace {
struct LessEqualOnly {
	int value;
	friend bool operator<=(const LessEqualOnly &left, const LessEqualOnly &right) {
		return left.value <= right.value;
	}
};

void verify(const Treap<int> &tree, const std::multiset<int> &expected, int key) {
	assert(tree.size() == int(expected.size()) && tree.empty() == expected.empty());
	const auto lower = expected.lower_bound(key);
	const auto upper = expected.upper_bound(key);
	assert(tree.count(key) == int(expected.count(key)));
	assert(tree.contains(key) == expected.contains(key));
	assert(tree.order_of_key(key) == std::distance(expected.begin(), lower));
	assert(tree.count_less_equal(key) == std::distance(expected.begin(), upper));
	assert(tree.lower_bound(key) == (lower == expected.end() ? std::nullopt : std::optional<int>(*lower)));
	assert(tree.upper_bound(key) == (upper == expected.end() ? std::nullopt : std::optional<int>(*upper)));
	assert(tree.max_less_equal(key) == (upper == expected.begin() ? std::nullopt : std::optional<int>(*std::prev(upper))));
	assert(!tree.kth(0) && !tree.kth(tree.size() + 1));
}
}  // namespace

int main() {
	std::mt19937 random(61503);
	Treap<int> tree;
	std::multiset<int> expected;
	for (int step = 0; step < 20000; ++step) {
		const int key = int(random() % 101) - 50;
		switch (random() % 4) {
			case 0:
			case 1:
				tree.insert(key);
				expected.insert(key);
				break;
			case 2: {
				const auto position = expected.find(key);
				assert(tree.erase_one(key) == (position != expected.end()));
				if (position != expected.end()) expected.erase(position);
				break;
			}
			case 3:
				assert(tree.erase_all(key) == int(expected.erase(key)));
				break;
		}
		verify(tree, expected, key);
		const int left = int(random() % 101) - 50;
		const int right = int(random() % 101) - 50;
		assert(tree.count_range(left, right) ==
		       (left <= right ? std::distance(expected.lower_bound(left), expected.upper_bound(right)) : 0));
		if (!expected.empty()) {
			const int rank = 1 + random() % expected.size();
			assert(tree.find_by_order(rank) == *std::next(expected.begin(), rank - 1));
		}
	}

	constexpr int lowest = std::numeric_limits<int>::lowest();
	constexpr int highest = std::numeric_limits<int>::max();
	tree.insert(lowest);
	tree.insert(highest);
	tree.insert(highest);
	Treap<int> copy(tree);
	assert(copy.erase_all(highest) == 2 && tree.count(highest) == 2);
	assert(tree.count_range(lowest, highest) == tree.size());
	Treap<int> assigned;
	assigned = tree;
	assigned.erase_one(lowest);
	assert(tree.contains(lowest) && !assigned.contains(lowest));
	Treap<int> moved(std::move(copy));
	assert(copy.empty() && moved.contains(lowest));
	tree.clear();
	assert(tree.empty() && moved.contains(lowest));

	Treap<LessEqualOnly> compatible;
	compatible.insert({4});
	compatible.insert({2});
	assert(compatible.find_by_order(1).value == 2 && compatible.contains({4}));
}
