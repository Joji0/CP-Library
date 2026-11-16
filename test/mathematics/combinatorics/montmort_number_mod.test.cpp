#define PROBLEM "https://judge.yosupo.jp/problem/montmort_number_mod"
#include <iostream>
#include <vector>
#include "mathematics/combinatorics/derangement_number.hpp"
#include "mathematics/modular_arithmetic/dynamic_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = DynamicModularInteger<0>;
	int n, modulus;
	std::cin >> n >> modulus;
	Modular::set_modulus(modulus);
	auto result = derangement_numbers<Modular>(n);
	for (int i = 1; i < int(result.size()); ++i) {
		if (i > 1) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
