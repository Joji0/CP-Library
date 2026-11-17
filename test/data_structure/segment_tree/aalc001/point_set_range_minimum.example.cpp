// https://atcoder.jp/contests/aalc001/tasks/aalc001_a
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/minimum.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<long long> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	SegmentTree<Minimum<>> tree(values);
	while (query_count--) {
		int type, left, right;
		std::cin >> type >> left >> right;
		if (type == 1)
			tree.set(left, right);
		else
			std::cout << tree.query(left, right) << '\n';
	}
}
