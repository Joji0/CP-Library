#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_F"
#include <iostream>
#include <limits>
#include "acted_monoid/minimum_assign.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int size, queries;
	std::cin >> size >> queries;
	LazySegmentTree<MinimumAssign<int>> tree(size, std::numeric_limits<int>::max());
	while (queries--) {
		int type, left, right;
		std::cin >> type >> left >> right;
		if (type == 0) {
			int value;
			std::cin >> value;
			tree.update(left + 1, right + 1, value);
		} else {
			std::cout << tree.query(left + 1, right + 1).value() << '\n';
		}
	}
}
