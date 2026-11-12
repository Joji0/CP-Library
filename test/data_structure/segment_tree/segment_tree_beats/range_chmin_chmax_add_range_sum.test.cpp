#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"
#include <iostream>
#include <vector>
#include "data_structure/segment_tree/segment_tree_beats.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n, queries;
	std::cin >> n >> queries;
	std::vector<long long> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	SegmentTreeBeats<long long> tree(values);
	while (queries--) {
		int type, left, right;
		std::cin >> type >> left >> right;
		++left;
		if (type == 3) {
			std::cout << tree.sum(left, right) << '\n';
			continue;
		}
		long long value;
		std::cin >> value;
		if (type == 0) tree.chmin(left, right, value);
		if (type == 1) tree.chmax(left, right, value);
		if (type == 2) tree.add(left, right, value);
	}
}
