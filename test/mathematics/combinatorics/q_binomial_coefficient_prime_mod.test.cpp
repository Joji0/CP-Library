#define PROBLEM "https://judge.yosupo.jp/problem/q_binomial_coefficient_prime_mod"
#include <iostream>
#include <vector>
#include "mathematics/combinatorics/q_binomial_coefficient.hpp"
#include "mathematics/modular_arithmetic/dynamic_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = DynamicModularInteger<0>;
	int count, modulus, maximum = 0;
	int base;
	std::cin >> count >> modulus >> base;
	std::vector<std::pair<int, int>> queries(count);
	for (auto& [n, r] : queries) {
		std::cin >> n >> r;
		maximum = std::max(maximum, n);
	}
	Modular::set_modulus(modulus);
	QBinomialCoefficient<Modular> combinations(base, maximum);
	for (const auto& [n, r] : queries) std::cout << combinations.binomial_coefficient(n, r) << '\n';
}
