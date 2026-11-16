#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/modular_square_root.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	std::cin >> count;
	while (count--) {
		long long value, prime;
		std::cin >> value >> prime;
		std::cout << ModularSquareRoot::solve(value, prime) << '\n';
	}
}
