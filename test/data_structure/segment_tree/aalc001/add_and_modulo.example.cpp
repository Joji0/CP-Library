// https://atcoder.jp/contests/aalc001/tasks/aalc001_h
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/add_modulo_composition.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Monoid = AddModuloComposition<998244353>;
	auto make_leaf = [](long long value) {
		return value == 0 ? Monoid::make_modulo() : Monoid::make_add(value);
	};
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Monoid::value_type> leaves(n + 1);
	for (int i = 1; i <= n; ++i) {
		long long value;
		std::cin >> value;
		leaves[i] = make_leaf(value);
	}
	SegmentTree<Monoid> tree(leaves);
	while (query_count--) {
		int type, left, argument;
		std::cin >> type >> left >> argument;
		if (type == 1)
			tree.set(left, make_leaf(argument));
		else {
			long long start;
			std::cin >> start;
			std::cout << Monoid::evaluate(tree.query(left, argument), start) << '\n';
		}
	}
}
