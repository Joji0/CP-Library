// https://atcoder.jp/contests/aalc001/tasks/aalc001_m
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/bitwise_function.hpp"
#include "monoid/reverse_monoid.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Function = BitwiseFunction<unsigned>;
	constexpr unsigned mask = (1U << 30) - 1;
	auto make_nand = [](unsigned value) -> Function::value_type { return {mask, mask ^ value}; };
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<unsigned> raw(n + 1);
	std::vector<Function::value_type> leaves(n + 1);
	for (int i = 1; i <= n; ++i) {
		std::cin >> raw[i];
		leaves[i] = make_nand(raw[i]);
	}
	SegmentTree<ReverseMonoid<Function>> tree(leaves);
	while (query_count--) {
		int type, left;
		unsigned argument;
		std::cin >> type >> left >> argument;
		if (type == 1) {
			raw[left] = argument;
			tree.set(left, make_nand(argument));
		} else
			std::cout << Function::evaluate(tree.query(left + 1, int(argument)), raw[left]) << '\n';
	}
}
