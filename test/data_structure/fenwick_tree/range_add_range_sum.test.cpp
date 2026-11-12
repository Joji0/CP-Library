#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/DSL_2_G"
#include <iostream>
#include "data_structure/fenwick_tree/range_fenwick_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	std::cin >> size >> queries;
	RangeFenwickTree<long long> tree(size);
	while (queries--) {
		int type, left, right;
		std::cin >> type >> left >> right;
		if (type == 0) {
			long long delta;
			std::cin >> delta;
			tree.range_add(left, right, delta);
		} else {
			std::cout << tree.range_sum(left, right) << '\n';
		}
	}
}
