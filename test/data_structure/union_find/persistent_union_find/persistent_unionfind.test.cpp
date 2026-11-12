#define PROBLEM "https://judge.yosupo.jp/problem/persistent_unionfind"
#include <iostream>
#include <vector>
#include "data_structure/union_find/persistent_union_find.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int size, queries;
	std::cin >> size >> queries;
	PersistentUnionFind sets(size);
	std::vector<int> version(queries + 1, 1);
	for (int query = 1; query <= queries; ++query) {
		int type, previous, first, second;
		std::cin >> type >> previous >> first >> second;
		if (type == 0)
			version[query] = sets.merge(version[previous + 1], first + 1, second + 1);
		else
			std::cout << sets.same(version[previous + 1], first + 1, second + 1) << '\n';
	}
}
