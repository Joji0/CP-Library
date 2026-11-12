#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"
#include <iostream>
#include "data_structure/union_find/union_find.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int size, queries;
	std::cin >> size >> queries;
	UnionFind sets(size);
	while (queries--) {
		int type, first, second;
		std::cin >> type >> first >> second;
		if (type == 0)
			sets.merge(first + 1, second + 1);
		else
			std::cout << sets.same(first + 1, second + 1) << '\n';
	}
}
