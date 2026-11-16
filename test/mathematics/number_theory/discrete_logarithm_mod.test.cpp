#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/discrete_logarithm.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	std::cin >> count;
	while (count--) {
		long long base, value, modulus;
		std::cin >> base >> value >> modulus;
		std::cout << DiscreteLogarithm::solve(base, value, modulus) << '\n';
	}
}
