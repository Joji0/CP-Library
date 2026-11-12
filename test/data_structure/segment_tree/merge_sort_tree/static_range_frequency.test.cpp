#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#include <iostream>
#include <vector>
#include "data_structure/segment_tree/merge_sort_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	if (!(std::cin >> size >> queries)) return 0;
	std::vector<int> values(size + 1);
	for (int i = 1; i <= size; ++i) std::cin >> values[i];
	const MergeSortTree<int> tree(values);
	while (queries--) {
		int left, right, value;
		std::cin >> left >> right >> value;
		std::cout << tree.count_equal(left + 1, right, value) << '\n';
	}
}
