// https://atcoder.jp/contests/aalc001/tasks/aalc001_n
#include <bits/stdc++.h>
#include "acted_monoid/minimum_weight_sum_add.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Acted = MinimumWeightSumAdd<int, Modular>;
	int n, query_count;
	std::cin >> n >> query_count;
	struct Event {
		int left, right, delta;
	};
	std::vector<std::vector<Event>> events(n + 2);
	for (int i = 0; i < query_count; ++i) {
		int top, left, bottom, right;
		std::cin >> top >> left >> bottom >> right;
		events[top].push_back({left, right, 1});
		events[bottom + 1].push_back({left, right, -1});
	}
	std::vector<Acted::Value> leaves(n + 1);
	for (int column = 1; column <= n; ++column) leaves[column] = Acted::make_leaf(0, column);
	LazySegmentTree<Acted> tree(leaves);
	Modular answer = 0;
	for (int row = 1; row <= n; ++row) {
		for (auto [left, right, delta] : events[row]) tree.update(left, right, delta);
		const auto whole = tree.all_product();
		if (whole.minimum == 0) answer += Modular(row) * whole.weight_sum;
	}
	std::cout << answer << '\n';
}
