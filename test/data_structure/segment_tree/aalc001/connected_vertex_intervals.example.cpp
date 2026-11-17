// https://atcoder.jp/contests/aalc001/tasks/aalc001_v
#include <bits/stdc++.h>
#include "acted_monoid/minimum_threshold_history_add.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Acted = MinimumThresholdHistoryAdd<>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<std::vector<int>> earlier(n + 1);
	for (int i = 1; i < n; ++i) {
		int a, b;
		std::cin >> a >> b;
		earlier[b].push_back(a);
	}
	std::vector<std::vector<std::pair<int, int>>> queries(n + 1);
	std::vector<long long> answers(query_count);
	for (int q = 0; q < query_count; ++q) {
		int left, right;
		std::cin >> left >> right;
		queries[right].emplace_back(left, q);
	}
	LazySegmentTree<Acted> tree(n, Acted::make_leaf(n + 1));
	for (int right = 1; right <= n; ++right) {
		tree.update(1, right - 1, Acted::make_add(1));
		for (int previous : earlier[right]) tree.update(1, previous, Acted::make_add(-1));
		tree.set_value(right, 1LL);
		tree.update(1, n, Acted::make_observe(1));
		for (auto [left, ordinal] : queries[right]) answers[ordinal] = tree.query(left, right).historical_sum;
	}
	for (auto answer : answers) std::cout << answer << '\n';
}
