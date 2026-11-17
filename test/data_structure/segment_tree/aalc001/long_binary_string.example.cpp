// https://atcoder.jp/contests/aalc001/tasks/aalc001_j
#include <bits/stdc++.h>
#include "acted_monoid/flip_sum.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Acted = FlipSum<>;
	int query_count;
	std::cin >> query_count;
	struct Query {
		int type;
		long long left, right;
	};
	std::vector<Query> queries(query_count);
	std::vector<long long> boundaries;
	for (auto &[type, left, right] : queries) {
		std::cin >> type >> left >> right;
		boundaries.push_back(left);
		boundaries.push_back(right + 1);
	}
	std::sort(boundaries.begin(), boundaries.end());
	boundaries.erase(std::unique(boundaries.begin(), boundaries.end()), boundaries.end());
	std::vector<Acted::Value> leaves(boundaries.size());
	for (std::size_t i = 1; i < boundaries.size(); ++i) leaves[i] = {0, boundaries[i] - boundaries[i - 1]};
	LazySegmentTree<Acted> tree(leaves);
	auto index = [&](long long x) { return int(std::lower_bound(boundaries.begin(), boundaries.end(), x) - boundaries.begin()); };
	for (auto [type, left, right] : queries) {
		const int l = index(left) + 1, r = index(right + 1);
		if (type == 1)
			tree.update(l, r, true);
		else
			std::cout << tree.query(l, r).sum << '\n';
	}
}
