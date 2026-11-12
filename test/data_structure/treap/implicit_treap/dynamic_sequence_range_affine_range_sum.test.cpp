#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"
#include <iostream>
#include <vector>
#include "acted_monoid/sum_affine.hpp"
#include "data_structure/treap/implicit_treap.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	using Modular = ModularInteger998244353;
	int size, queries;
	std::cin >> size >> queries;
	std::vector<Modular> values(size + 1);
	for (int i = 1; i <= size; ++i) std::cin >> values[i];
	auto sequence = ImplicitTreap<SumAffine<Modular>>::from_values(values);
	while (queries--) {
		int operation, left;
		std::cin >> operation >> left;
		if (operation == 0) {
			Modular value;
			std::cin >> value;
			sequence.insert_value(left + 1, value);
		} else if (operation == 1) {
			sequence.erase(left + 1);
		} else {
			int right;
			std::cin >> right;
			if (operation == 2) {
				sequence.reverse(left + 1, right);
			} else if (operation == 3) {
				Modular multiplier, offset;
				std::cin >> multiplier >> offset;
				sequence.apply(left + 1, right, {multiplier, offset});
			} else {
				std::cout << sequence.product(left + 1, right).val << '\n';
			}
		}
	}
}
