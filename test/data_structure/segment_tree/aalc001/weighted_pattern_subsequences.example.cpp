// https://atcoder.jp/contests/aalc001/tasks/aalc001_e
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "monoid/weighted_pattern_subsequences.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Monoid = WeightedPatternSubsequences<std::array{'R', 'G', 'B'}, Modular>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Modular> weights(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> weights[i];
	std::string symbols;
	std::cin >> symbols;
	symbols.insert(symbols.begin(), ' ');
	std::vector<Monoid::value_type> values(n + 1);
	for (int i = 1; i <= n; ++i) values[i] = Monoid::make_leaf(symbols[i], weights[i]);
	SegmentTree<Monoid> tree(values);
	while (query_count--) {
		int type, left, value;
		std::cin >> type >> left >> value;
		if (type == 1)
			tree.set(left, Monoid::make_leaf(symbols[left], Modular(value)));
		else
			std::cout << Monoid::weighted_count(tree.query(left, value)) << '\n';
	}
}
