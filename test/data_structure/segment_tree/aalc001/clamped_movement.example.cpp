// https://atcoder.jp/contests/aalc001/tasks/aalc001_q
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/clamped_movement_composition.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Monoid = ClampedMovementComposition;
	int n, query_count;
	long long maximum;
	std::cin >> n >> maximum >> query_count;
	std::string directions;
	std::cin >> directions;
	directions.insert(directions.begin(), ' ');
	std::vector<long long> steps(n + 1);
	std::vector<Monoid::value_type> leaves(n + 1);
	auto make_leaf = [&](int i) { return Monoid::make_shift(directions[i] == '0' ? -steps[i] : steps[i], 0, maximum); };
	for (int i = 1; i <= n; ++i) {
		std::cin >> steps[i];
		leaves[i] = make_leaf(i);
	}
	SegmentTree<Monoid> tree(leaves);
	while (query_count--) {
		int type;
		long long argument;
		std::cin >> type >> argument;
		if (type == 1) {
			const int position = int(argument);
			directions[position] = directions[position] == '0' ? '1' : '0';
			tree.set(position, make_leaf(position));
		} else {
			int left, right;
			std::cin >> left >> right;
			std::cout << Monoid::evaluate(tree.query(left, right), argument).distance << '\n';
		}
	}
}
