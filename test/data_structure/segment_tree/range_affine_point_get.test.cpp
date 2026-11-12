#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"
#include <iostream>
#include <vector>
#include "data_structure/segment_tree/dual_segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "monoid/affine.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	using Function = Affine<ModularInteger998244353>;
	int n, queries;
	std::cin >> n >> queries;
	std::vector<Function::value_type> values(n + 1);
	for (int i = 1; i <= n; ++i) {
		// Represent the initial point value as a constant function.
		std::cin >> values[i].b;
	}
	DualSegmentTree<Function> tree(values);
	while (queries--) {
		int type;
		std::cin >> type;
		if (type == 0) {
			int left, right;
			Function::value_type update;
			std::cin >> left >> right >> update.a >> update.b;
			tree.apply(left + 1, right, update);
		} else {
			int position;
			std::cin >> position;
			std::cout << tree.get(position + 1).b << '\n';
		}
	}
}
