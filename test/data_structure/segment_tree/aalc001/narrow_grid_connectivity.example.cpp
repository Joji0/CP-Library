// https://atcoder.jp/contests/aalc001/tasks/aalc001_r
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/grid_boundary_connectivity.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Monoid = GridBoundaryConnectivity<10>;
	int n, width, query_count;
	std::cin >> n >> width >> query_count;
	auto read_row = [] {
		std::string row;
		std::cin >> row;
		row.insert(row.begin(), ' ');
		return Monoid::make_leaf(row);
	};
	std::vector<Monoid::value_type> rows(n + 1);
	for (int i = 1; i <= n; ++i) rows[i] = read_row();
	SegmentTree<Monoid> tree(rows);
	while (query_count--) {
		int type, first_row;
		std::cin >> type >> first_row;
		if (type == 1)
			tree.set(first_row, read_row());
		else {
			int first_column, last_row, last_column;
			std::cin >> first_column >> last_row >> last_column;
			if (first_row > last_row) {
				std::swap(first_row, last_row);
				std::swap(first_column, last_column);
			}
			const bool connected = Monoid::connected_with_context(tree.query(1, first_row - 1),
									      tree.query(first_row, last_row), tree.query(last_row + 1, n), first_column, last_column);
			std::cout << (connected ? "Yes" : "No") << '\n';
		}
	}
}
