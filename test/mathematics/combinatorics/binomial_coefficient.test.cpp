#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient"
#include <iostream>
#include <vector>
#include "mathematics/combinatorics/composite_modular_binomial_coefficient.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	long long modulus;
	std::cin >> count >> modulus;
	CompositeModularBinomialCoefficient combinations(modulus);
	while (count--) {
		long long n, r;
		std::cin >> n >> r;
		std::cout << combinations.binomial_coefficient(n, r) << '\n';
	}
}
