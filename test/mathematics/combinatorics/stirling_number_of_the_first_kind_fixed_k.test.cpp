#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind_fixed_k"
#include <iostream>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/combinatorics/stirling_number_of_the_first_kind_fixed_cycles.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n, cycles;
	std::cin >> n >> cycles;
	auto result = stirling_numbers_of_the_first_kind_fixed_cycles<Modular>(n, cycles, true);
	for (int i = cycles; i < int(result.size()); ++i) {
		if (i > cycles) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
