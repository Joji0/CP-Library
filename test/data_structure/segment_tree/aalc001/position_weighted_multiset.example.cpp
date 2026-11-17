// https://atcoder.jp/contests/aalc001/tasks/aalc001_k
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "monoid/position_weighted_sum.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Monoid = PositionWeightedSum<Modular>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<int> initial(n), keys;
	for (auto &value : initial) std::cin >> value;
	keys = initial;
	std::vector<std::pair<int, int>> queries(query_count);
	for (auto &[type, value] : queries) {
		std::cin >> type;
		if (type != 3) {
			std::cin >> value;
			keys.push_back(value);
		}
	}
	std::sort(keys.begin(), keys.end());
	keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
	auto index = [&](int value) { return int(std::lower_bound(keys.begin(), keys.end(), value) - keys.begin()) + 1; };
	std::vector<long long> count(keys.size() + 1);
	for (auto value : initial) ++count[index(value)];
	std::vector<Monoid::value_type> leaves(keys.size() + 1);
	for (std::size_t i = 1; i < count.size(); ++i) leaves[i] = Monoid::make_repeated(keys[i - 1], count[i]);
	SegmentTree<Monoid> tree(leaves);
	for (auto [type, value] : queries) {
		if (type == 3)
			std::cout << tree.all_product().weighted_sum << '\n';
		else {
			const int position = index(value);
			count[position] += type == 1 ? 1 : -1;
			tree.set(position, Monoid::make_repeated(value, count[position]));
		}
	}
}
