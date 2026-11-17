// https://atcoder.jp/contests/aalc001/tasks/aalc001_d
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/finite_alphabet_inversions.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Monoid = FiniteAlphabetInversions<3>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Monoid::value_type> values(n + 1);
	for (int i = 1; i <= n; ++i) {
		int value;
		std::cin >> value;
		values[i] = Monoid::make_leaf(value);
	}
	SegmentTree<Monoid> tree(values);
	while (query_count--) {
		int type, left;
		int value;
		std::cin >> type >> left >> value;
		if (type == 1)
			tree.set(left, Monoid::make_leaf(value));
		else {
			auto aggregate = tree.query(left, static_cast<int>(value));
			std::cout << aggregate.inversions << '\n';
		}
	}
}
