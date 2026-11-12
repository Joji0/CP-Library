#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include <iostream>
#include <vector>
#include "data_structure/fenwick_tree/fenwick_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n, queries;
	std::cin >> n >> queries;
	std::vector<long long> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	FenwickTree<long long> tree(values);
	while (queries--) {
		int type, left;
		std::cin >> type >> left;
		if (type == 0) {
			long long delta;
			std::cin >> delta;
			tree.add(left + 1, delta);
		} else {
			int right;
			std::cin >> right;
			std::cout << tree.sum(left + 1, right) << '\n';
		}
	}
}
