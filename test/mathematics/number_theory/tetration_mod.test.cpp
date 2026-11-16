#define PROBLEM "https://judge.yosupo.jp/problem/tetration_mod"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/modular_tetration.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int count;
	std::cin >> count;
	while (count--) {
		long long base, modulus;
		uint64_t height;
		std::cin >> base >> height >> modulus;
		std::cout << modular_tetration(base, height, modulus) << '\n';
	}
}
