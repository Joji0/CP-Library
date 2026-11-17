// https://atcoder.jp/contests/aalc001/tasks/aalc001_b
#include <bits/stdc++.h>
#include "acted_monoid/minimum_add.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<long long> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	LazySegmentTree<MinimumAdd<>> tree(values);
	while (query_count--) {
		int type, left, right;
		std::cin >> type >> left >> right;
		if (type == 1) {
			long long delta;
			std::cin >> delta;
			tree.update(left, right, delta);
		} else
			std::cout << *tree.query(left, right) << '\n';
	}
}
