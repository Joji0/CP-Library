#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <iostream>
#include <vector>
#include "data_structure/sparse_table/sparse_table.hpp"
#include "monoid/minimum.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	if (!(std::cin >> size >> queries)) return 0;
	std::vector<int> values(size + 1);
	for (int position = 1; position <= size; ++position) std::cin >> values[position];
	const SparseTable<Minimum<int>> minimum(values);
	while (queries--) {
		int left, right;
		std::cin >> left >> right;
		std::cout << minimum.query(left + 1, right) << '\n';
	}
}
