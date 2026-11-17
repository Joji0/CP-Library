// https://atcoder.jp/contests/aalc001/tasks/aalc001_o
#include <bits/stdc++.h>
#include "acted_monoid/historical_sum_add_assign.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Acted = HistoricalSumAddAssign<>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<long long> values(n + 1), answers(query_count);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	std::vector<std::vector<std::pair<int, int>>> queries(n + 1);
	for (int q = 0; q < query_count; ++q) {
		int left, right;
		std::cin >> left >> right;
		queries[right].emplace_back(left, q);
	}
	LazySegmentTree<Acted> tree(n);
	std::vector<int> increasing;
	for (int right = 1; right <= n; ++right) {
		while (!increasing.empty() && values[increasing.back()] >= values[right]) increasing.pop_back();
		const int left = increasing.empty() ? 1 : increasing.back() + 1;
		tree.update(left, right, Acted::make_assign(values[right]));
		tree.update(1, n, Acted::make_accumulate());
		for (auto [query_left, ordinal] : queries[right]) answers[ordinal] = tree.query(query_left, right).historical_sum;
		increasing.push_back(right);
	}
	for (auto answer : answers) std::cout << answer << '\n';
}
