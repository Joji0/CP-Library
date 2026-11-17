// https://atcoder.jp/contests/aalc001/tasks/aalc001_c
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/add.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<long long> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	SegmentTree<Add<>> tree(values);
	while (query_count--) {
		int type, position;
		long long argument;
		std::cin >> type >> position >> argument;
		auto below = [&](long long sum) { return sum < argument; };
		if (type == 1)
			tree.set(position, argument);
		else if (type == 2)
			std::cout << tree.max_right(position, below) + 1 << '\n';
		else
			std::cout << tree.min_left(position, below) - 1 << '\n';
	}
}
