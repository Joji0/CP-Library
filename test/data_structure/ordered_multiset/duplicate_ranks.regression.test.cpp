#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <optional>
#include <random>
#include <set>
#include "data_structure/ordered_multiset.hpp"
#include "data_structure/ordered_set.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
struct Compare {
	explicit Compare(bool descending) : descending(descending) {}
	bool operator()(int first, int second) const { return descending ? second < first : first < second; }

	bool descending;
};

void check(bool descending) {
	const Compare compare(descending);
	OrderedMultiset<int, Compare> values(compare);
	std::multiset<int, Compare> expected(compare);
	for (int value : {1, 2, 2}) {
		values.insert(value);
		expected.insert(value);
	}
	const int left = descending ? 1 : 3, right = descending ? 3 : 1;
	assert(values.count_halfopen(left, right) == 0);
	assert(values.count_range(left, right) == 0 && values.count_halfopen(2, 2) == 0);
	assert(values.count(2) == 2);

	std::mt19937 random(descending ? 719 : 1907);
	for (int step = 0; step < 2500; ++step) {
		const int value = int(random() % 41) - 20;
		switch (random() % 4) {
			case 0:
				values.insert(value);
				expected.insert(value);
				break;
			case 1: {
				const auto position = expected.find(value);
				assert(values.erase_one(value) == (position != expected.end()));
				if (position != expected.end()) expected.erase(position);
				break;
			}
			case 2:
				assert(values.erase_all(value) == int(expected.erase(value)));
				break;
			case 3: {
				const int rank = int(random() % (expected.size() + 2));
				const auto removed = values.erase_kth(rank);
				if (rank < 1 || int(expected.size()) < rank) {
					assert(!removed);
				} else {
					const auto position = std::next(expected.begin(), rank - 1);
					assert(removed == *position);
					expected.erase(position);
				}
				break;
			}
		}
		const int bound = int(random() % 45) - 22;
		const int other = int(random() % 45) - 22;
		const auto lower = expected.lower_bound(bound);
		const auto upper = expected.upper_bound(bound);
		const int less = int(std::distance(expected.begin(), lower));
		const int less_equal = int(std::distance(expected.begin(), upper));
		assert(values.size() == int(expected.size()) && values.empty() == expected.empty());
		assert(values.order_of_key(bound) == less && values.count_less_equal(bound) == less_equal);
		assert(values.count(bound) == int(expected.count(bound)));
		assert(values.lower_bound(bound) == (lower == expected.end() ? std::nullopt : std::optional(*lower)));
		assert(values.upper_bound(bound) == (upper == expected.end() ? std::nullopt : std::optional(*upper)));
		assert(values.count_halfopen(bound, other) ==
		       (compare(bound, other) ? int(std::distance(lower, expected.lower_bound(other))) : 0));
		assert(values.count_range(bound, other) ==
		       (compare(other, bound) ? 0 : int(std::distance(lower, expected.upper_bound(other)))));
		int rank = 1;
		for (int current : expected) assert(values.kth(rank++) == current);
		assert(!values.kth(0) && !values.kth(rank));
	}
	values.clear();
	assert(values.empty() && !values.first() && !values.last());

	OrderedSet<int, Compare> unique(compare);
	assert(unique.insert(1) && unique.insert(3) && !unique.insert(3));
	assert(unique.count_halfopen(left, right) == 0 && unique.count_range(left, right) == 0);
	assert(unique.first() == (descending ? 3 : 1));
}
}  // namespace

int main() {
	check(false);
	check(true);
	answer_library_checker_aplusb();
}
