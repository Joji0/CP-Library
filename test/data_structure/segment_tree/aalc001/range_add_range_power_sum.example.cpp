// https://atcoder.jp/contests/aalc001/tasks/aalc001_g
#include <bits/stdc++.h>
#include "acted_monoid/power_sums_add.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Modular> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	LazySegmentTree<PowerSumsAdd<Modular, 4>> tree(values);
	while (query_count--) {
		int type, left, right, argument;
		std::cin >> type >> left >> right >> argument;
		if (type == 1)
			tree.update(left, right, argument);
		else
			std::cout << tree.query(left, right).powers[argument] << '\n';
	}
}
