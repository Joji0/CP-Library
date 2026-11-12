#define PROBLEM "https://judge.yosupo.jp/problem/range_reverse_range_sum"
#include <iostream>
#include <vector>
#include "acted_monoid/sum_add.hpp"
#include "data_structure/treap/implicit_treap.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	std::cin >> size >> queries;
	std::vector<long long> values(size + 1);
	for (int i = 1; i <= size; ++i) std::cin >> values[i];
	auto sequence = ImplicitTreap<SumAdd<long long>>::from_values(values);
	while (queries--) {
		int operation, left, right;
		std::cin >> operation >> left >> right;
		if (operation == 0)
			sequence.reverse(left + 1, right);
		else
			std::cout << sequence.product(left + 1, right).val << '\n';
	}
}
