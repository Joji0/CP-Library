#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_fixed_k"
#include <iostream>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/combinatorics/stirling_number_of_the_second_kind_fixed_blocks.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n, block_count;
	std::cin >> n >> block_count;
	auto result = stirling_numbers_of_the_second_kind_fixed_blocks<Modular>(n, block_count);
	for (int i = block_count; i < int(result.size()); ++i) {
		if (i > block_count) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
