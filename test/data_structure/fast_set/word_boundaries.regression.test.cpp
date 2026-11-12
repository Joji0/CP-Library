#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <iterator>
#include <random>
#include <set>
#include <string>
#include <vector>
#include "data_structure/fast_set.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	FastSet empty(0);
	empty.build("0");
	assert(empty.empty() && empty.next(1) == -1 && empty.prev(1) == -1);
	assert(!empty.any(1, 0));

	std::mt19937 random(827);
	for (int size : {1, 63, 64, 65, 127, 128, 129, 4095, 4096, 4097}) {
		FastSet fast(size);
		std::set<int> expected;
		std::string bits(size + 1, '0');
		for (int position = 1; position <= size; position += 9) {
			bits[position] = '1';
			expected.insert(position);
		}
		fast.build(bits);
		for (int step = 0; step < 600; ++step) {
			const int position = 1 + random() % size;
			if (step % 2 == 0) {
				fast.insert(position);
				expected.insert(position);
			} else {
				fast.erase(position);
				expected.erase(position);
			}
			const int bound = int(random() % (size + 2));
			const auto successor = expected.lower_bound(bound);
			const auto after_predecessor = expected.upper_bound(bound);
			assert(fast.contains(position) == expected.contains(position));
			assert(fast.next(bound) == (successor == expected.end() ? -1 : *successor));
			assert(fast.prev(bound) == (after_predecessor == expected.begin() ? -1 : *std::prev(after_predecessor)));
			assert(fast.empty() == expected.empty());
		}
		std::vector<int> reported;
		fast.enumerate(1, size, [&](int position) { reported.push_back(position); });
		assert(reported == std::vector<int>(expected.begin(), expected.end()));
		assert(!fast.any(size, size - 1));
		fast.clear();
		assert(fast.empty() && fast.min() == -1 && fast.max() == -1);
	}
	answer_library_checker_aplusb();
}
