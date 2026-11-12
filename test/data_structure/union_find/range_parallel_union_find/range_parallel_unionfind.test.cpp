#define PROBLEM "https://judge.yosupo.jp/problem/range_parallel_unionfind"
#include <iostream>
#include <vector>
#include "data_structure/union_find/range_parallel_union_find.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int size, queries;
	std::cin >> size >> queries;
	std::vector<Modular> sums(size + 1);
	for (int vertex = 1; vertex <= size; ++vertex) std::cin >> sums[vertex];
	RangeParallelUnionFind sets(size);
	Modular result = 0;
	while (queries--) {
		int count, first, second;
		std::cin >> count >> first >> second;
		sets.merge_pairs(first + 1, second + 1, count, [&](int root, int absorbed) {
			result += sums[root] * sums[absorbed];
			sums[root] += sums[absorbed];
		});
		std::cout << result << '\n';
	}
}
