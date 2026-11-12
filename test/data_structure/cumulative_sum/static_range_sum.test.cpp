#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include <iostream>
#include <vector>
#include "data_structure/cumulative_sum.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	if (!(std::cin >> size >> queries)) return 0;
	std::vector<long long> values(size + 1);
	for (int position = 1; position <= size; ++position) std::cin >> values[position];
	const CumulativeSum<long long> prefix(values);
	while (queries--) {
		int left, right;
		std::cin >> left >> right;
		std::cout << prefix.query(left + 1, right) << '\n';
	}
}
