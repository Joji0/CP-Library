#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential"
#include <iostream>
#include "data_structure/union_find/weighted_union_find.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int size, queries;
	std::cin >> size >> queries;
	WeightedUnionFind<ModularInteger998244353> sets(size);
	while (queries--) {
		int type, first, second;
		std::cin >> type >> first >> second;
		if (type == 0) {
			ModularInteger998244353 value;
			std::cin >> value;
			std::cout << sets.merge(second + 1, first + 1, value) << '\n';
		} else {
			const auto difference = sets.try_difference(second + 1, first + 1);
			if (difference)
				std::cout << *difference << '\n';
			else
				std::cout << "-1\n";
		}
	}
}
