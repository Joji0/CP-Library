// https://atcoder.jp/contests/aalc001/tasks/aalc001_s
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/maximum_plus_matrix.hpp"

using Transition = MaximumPlusMatrix<long long, 10>;

Transition::value_type make_leaf(long long value) {
	auto result = Transition::identity();
	auto sign = [](int index) { return 2 * index - 1; };
	// States: 1 = before any selection, 2..3 = first sign,
	// 4..7 = last two signs, 8..9 = one sign left, 10 = finished.
	for (int recent = 0; recent < 2; ++recent) {
		result[1][2 + recent] = sign(recent) * value;
		result[2 + recent][8 + recent] = 2 * sign(recent) * value;
		result[8 + recent][10] = 3 * sign(recent) * value;
		for (int next = 0; next < 2; ++next) {
			result[2 + recent][4 + 2 * recent + next] = (2 * sign(recent) + sign(next)) * value;
			for (int previous = 0; previous < 2; ++previous) {
				const int from = 4 + 2 * previous + recent, to = 4 + 2 * recent + next;
				const long long contribution = (3 * sign(previous) + 2 * sign(recent)) * value;
				result[from][8 + recent] = contribution;
				const long long selected = contribution + sign(next) * value;
				result[from][to] = from == to ? std::max(0LL, selected) : selected;
			}
		}
	}
	return result;
}
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Transition::value_type> leaves(n + 1);
	for (int i = 1; i <= n; ++i) {
		long long value;
		std::cin >> value;
		leaves[i] = make_leaf(value);
	}
	SegmentTree<Transition> tree(leaves);
	while (query_count--) {
		int type, left;
		long long argument;
		std::cin >> type >> left >> argument;
		if (type == 1)
			tree.set(left, make_leaf(argument));
		else
			std::cout << *tree.query(left, int(argument))[1][10] << '\n';
	}
}
