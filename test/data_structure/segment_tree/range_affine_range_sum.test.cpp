#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
#include <iostream>
#include <vector>
#include "acted_monoid/sum_affine.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	using Action = SumAffine<ModularInteger998244353>;
	int n, queries;
	std::cin >> n >> queries;
	std::vector<ModularInteger998244353> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	auto tree = LazySegmentTree<Action>::from_values(values);
	while (queries--) {
		int type, left, right;
		std::cin >> type >> left >> right;
		if (type == 0) {
			Action::Action update;
			std::cin >> update.a >> update.b;
			tree.update(left + 1, right, update);
		} else {
			std::cout << tree.query(left + 1, right).val << '\n';
		}
	}
}
